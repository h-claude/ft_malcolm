# Récap de session — ft_malcolm (ARP spoofing) + side-quest BPF

> À coller dans une nouvelle conversation pour reprendre avec tout le contexte.

---

## 1. Comment on travaille ensemble (directives à respecter)

- **Apprentissage d'abord** : ne jamais faire le travail / écrire le code à ma place. Me guider, poser des questions, donner des indices — c'est **moi** qui produis.
- **Expliquer les notions avec des images/analogies**, tout en utilisant le **vocabulaire technique** correct.
- **Ne pas griller les étapes** : mon niveau en cybersécurité/réseau/prog n'est pas encore élevé, avancer progressivement, vérifier la compréhension.
- Objectif long terme : devenir **très bon en cybersécurité, réseau et programmation**, et apprendre à **travailler efficacement avec l'IA** (l'IA comme partenaire de sparring / prof particulier, pas comme béquille qui code à ma place).

---

## 2. Le projet : ft_malcolm (42)

Outil d'**ARP spoofing** en C, compilé avec `gcc -Wall -Wextra -Werror`, utilise ma libft (`turbo_libft` en submodule). Tourne sous **Linux dans une VM Vagrant (Debian 12)**, répertoire de travail `/vagrant`.

**Arguments :** `<source_ip> <source_mac> <target_ip> <target_mac>`

### État du code
- **FAIT** : parsing des arguments (IP via `inet_pton`, MAC via conversion hex), `print_data`, gestion d'erreurs, `freetab`, sélection d'interface (`find_interface`, portée sur Linux : `AF_PACKET` / `sockaddr_ll` / `ARPHRD_ETHER`).
- **FAIT (en cours)** : création du socket `socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))` + un `recv` basique dans `launch_the_scam()` (src/main.c).
- **À FAIRE (le cœur)** dans `launch_the_scam()` :
  1. écouter (listen)
  2. recevoir une ARP **request** (le déclencheur)
  3. vérifier que c'est la bonne (check the request)
  4. forger la reply falsifiée (create ARP reply)
  5. l'envoyer (`sendto`)
  6. s'arrêter
- **À gérer aussi** : `sigaction` (SIGINT).
- **Point de vigilance** : le socket écoute *toutes* les interfaces alors qu'on en sélectionne une (on calcule `if_nametoindex` mais on ne bind pas). Pour ce sujet, ce n'est pas bloquant (on filtre en userspace).

### Comportement attendu (lu dans le sujet)
Le programme **attend une ARP request**, envoie **UNE** reply falsifiée, puis **s'arrête**. Donc **pas besoin de flood/boucle** (version minimale).

---

## 3. Fil pédagogique parcouru

### ARP (Address Resolution Protocol)
- Relie une **IP (couche 3)** ↔ une **MAC (couche 2)**. Fait le pont entre adresse logique et adresse physique.
- A veut parler à B, ne connaît pas sa MAC → envoie une **ARP request** en **broadcast** (`ff:ff:ff:ff:ff:ff`), opcode **1** (*who-has*).
- B répond par une **ARP reply** en **unicast**, opcode **2** (*is-at*). A (et B) met(tent) à jour son **cache ARP**.
- **Faille** : protocole **stateless, sans authentification** → n'importe qui peut répondre. Règle brutale du cache : **le dernier qui parle a raison** (la dernière reply écrase l'entrée).
- **ARP spoofing / cache poisoning** : E se fait passer pour B → A envoie son trafic à E → **MITM** (lecture si non chiffré, ou interruption).
- **Contrainte** : ne marche que sur le **réseau local** (même broadcast domain / LAN). L'attaquant doit être sur le même segment.

Dans ft_malcolm, mon programme joue le rôle de **E** : il **forge** (≠ "tronque") une ARP reply falsifiée.

### Structure d'une trame ARP sur Ethernet (offsets en octets)
```
0  : MAC destination (6)
6  : MAC source      (6)
12 : EtherType       (2)   -> 0x0806 pour ARP ; l'ARP commence à 14
14 : Hardware type   (2)
16 : Protocol type   (2)
18 : Hardware len    (1)
19 : Protocol len    (1)
20 : OPCODE          (2)   <- 1=request, 2=reply
22 : Sender MAC (SHA) (6)
28 : Sender IP  (SPA) (4)
32 : Target MAC (THA) (6)
38 : Target IP  (TPA) (4)
-> trame complète = 42 octets
```
À retenir : **opcode @20, sender MAC @22, sender IP @28**.

### Raw socket & test
- `socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))` : le 3e arg est un **filtre EtherType** côté noyau.
- Un raw socket exige les **droits root** (lancer en `sudo`).
- `recv` est **bloquant** → "je ne reçois rien" = le programme **attend** (normal), il faut générer du trafic ARP.
- Un socket `AF_PACKET` voit aussi les paquets **sortants** de la machine → **pas besoin d'une 2e VM** pour tester.
- Générer de l'ARP pour tester :
  - simple : `sudo ip neigh flush all` puis `ping -c1 <IP_locale>`
  - ciblé : `sudo arping -c1 <IP>` (ne fait que des requests)
  - contrôle total : **scapy** (forge request ET reply, tous les champs). Ex :
    ```python
    from scapy.all import Ether, ARP, sendp
    sendp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=1, psrc="10.0.2.5", pdst="10.0.2.9"))  # request
    sendp(Ether(dst="ff:ff:ff:ff:ff:ff")/ARP(op=2, psrc="10.0.2.5",
          hwsrc="aa:bb:cc:dd:ee:ff", pdst="10.0.2.9"))                                  # reply
    ```
- **Bonne pratique à venir** : décoder le buffer reçu en "plaquant" une `struct` dessus (comme `sockaddr_ll`) plutôt qu'avec des index magiques `buffer[6]`, `buffer[7]`...

### setsockopt — pourquoi c'est "disponible mais pas obligatoire"
- La liste de fonctions autorisées = une **boîte à outils**, pas une recette imposée.
- Usages utiles : `SO_BINDTODEVICE` (restreindre à une interface), `SO_RCVTIMEO` (timeout sur recv), `SO_ATTACH_FILTER` (attacher un filtre BPF).
- Ça marche sans, car : la request est un broadcast (reçue quand même) et l'interface d'envoi se précise dans `sockaddr_ll.sll_ifindex` de `sendto`.

### Filtrage : deux endroits possibles
1. **Noyau** (videur à l'entrée) : EtherType via le 3e arg du socket (déjà fait), ou **BPF** via `setsockopt(SO_ATTACH_FILTER)`.
2. **Userspace** (contrôle des papiers à l'intérieur) : des `if` dans mon code → c'est l'étape **"check the request"** attendue pour ft_malcolm.

---

## 4. Side-quest : écrire un filtre BPF (apprentissage perso, hors sujet)

### Pourquoi
Je veux me familiariser avec BPF plutôt que juste refaire un check userspace (déjà fait plein de fois). Je connais un peu l'**asm** (projet libasm fait).

### Modèle mental : cBPF = mini-CPU à accumulateur
- Registres : **A** (accumulateur), **X** (index), + mémoire scratch `M[0..15]`.
- La "mémoire" adressable = **le paquet**.
- Une instruction = `struct sock_filter { __u16 code; __u8 jt; __u8 jf; __u32 k; }`.
- `code` = classe OR modificateurs (comme mnémonique + mode d'adressage en asm).
- **Pièges** : pas de saut arrière / pas de boucle (le filtre doit terminer) ; `jt`/`jf` = **nombre d'instructions à sauter vers l'avant** (pas des labels) ; pas de `JLT`/`JLE`/`JNE` → on inverse `jt`/`jf`.
- `BPF_RET | BPF_K` : retourne le **nombre d'octets gardés** (0 = jette).

### Jeu d'instructions cBPF
- **Classes** : `BPF_LD`, `BPF_LDX`, `BPF_ST`, `BPF_STX`, `BPF_ALU`, `BPF_JMP`, `BPF_RET`, `BPF_MISC`.
- **Tailles** (LD/LDX) : `BPF_B` (1o), `BPF_H` (2o), `BPF_W` (4o).
- **Modes d'adressage** : `BPF_IMM` (A=k), `BPF_ABS` (A=pkt[k]), `BPF_IND` (A=pkt[X+k]), `BPF_MEM` (A=M[k]), `BPF_LEN` (A=longueur), `BPF_MSH` (A=4*(pkt[k]&0xf)).
- **ALU** : ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, LSH, RSH, NEG (source `BPF_K` ou `BPF_X`).
- **JMP** : `BPF_JA` (toujours, offset dans k), `BPF_JEQ`, `BPF_JGT`, `BPF_JGE`, `BPF_JSET` (test de bits) (source `BPF_K` ou `BPF_X`).
- **RET** : `BPF_RET|BPF_K`, `BPF_RET|BPF_A`.
- **MISC** : `BPF_TAX` (X=A), `BPF_TXA` (A=X).
- **Macros** : `BPF_STMT(code, k)` (non-saut), `BPF_JUMP(code, k, jt, jf)` (saut).
- Bonus avancé (non nécessaire ici) : offsets magiques `SKF_AD_OFF + SKF_AD_PKTTYPE/PROTOCOL/...`.

Exemple (garder uniquement ARP) :
```c
BPF_STMT(BPF_LD | BPF_H | BPF_ABS, 12)              // A = EtherType
BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0x0806, 0, 1)   // si ARP -> continue, sinon saute 1
// ... ret #accept / ret #0
```
Attaché via `setsockopt(fd, SOL_SOCKET, SO_ATTACH_FILTER, &prog, sizeof(prog))` avec
`struct sock_fprog { unsigned short len; struct sock_filter *filter; }`.

### Bonne pratique BPF
- **En prod** : personne n'écrit le bytecode à la main → on utilise **libpcap** (`pcap_compile` sur une string `"arp and src host X"`), ou `tcpdump -dd` pour un tableau figé.
- **Dans ft_malcolm** : libpcap n'est pas autorisé → à la main + `setsockopt` (mais BPF est optionnel ici).
- **Pour apprendre (mon cas)** : **écrire à la main** avec `BPF_STMT`/`BPF_JUMP`, et utiliser `tcpdump -d` / `tcpdump -dd` comme **corrigé** (exactement comme comparer son asm avec `gcc -S` en libasm).
  ```bash
  tcpdump -d  'arp'                        # lisible
  tcpdump -dd 'arp'                        # tableau C copiable
  tcpdump -d  'arp and src host 10.0.2.5'  # comparer pour voir l'encodage du test d'IP
  ```

### Bonus culture (sécu)
cBPF (classic, filtrage de paquets) vs **eBPF** (extended) : VM généralisée dans le noyau, omniprésente en sécu/observabilité (seccomp, XDP, Falco, Cilium). Mot-clé à retenir : **eBPF**.

---

## 5. Prochaine étape

**Échelle de difficulté BPF :**
1. Échauffement : filtre "ARP requests seulement" (teste l'opcode @20 == 1) ; vérifier qu'une reply (op=2) ne passe plus.
2. Cœur : ajouter "Sender IP (@28) == IP en dur".
3. Boss : rendre l'IP **dynamique** en patchant le champ `k` au runtime depuis l'argument (attention au *network byte order* → d'où l'intérêt de générer avec tcpdump pour voir la bonne valeur).

**Tâche immédiate :** écrire à la main le petit filtre "ARP seulement" (4 instructions), puis le comparer à `tcpdump -dd 'arp'` instruction par instruction.

---

## Infra (note)
La mémoire de Claude Code est stockée sur le dossier partagé `/vagrant/.claude-memory/` (symlink depuis `~/.claude/projects/-vagrant/memory`, recréé par le Vagrantfile, ignoré par git) pour survivre aux réinstalls de VM.
