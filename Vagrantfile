# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "koalephant/debian12"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = 2048
    vb.cpus = 2
  end

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y build-essential
  SHELL

  # Keep Claude Code's memory on the shared folder so it survives VM rebuilds.
  # Re-links ~/.claude/projects/-vagrant/memory -> /vagrant/.claude-memory
  config.vm.provision "shell", privileged: false, run: "always", inline: <<-SHELL
    STORE=/vagrant/.claude-memory
    MEMDIR="$HOME/.claude/projects/-vagrant/memory"
    mkdir -p "$STORE"
    mkdir -p "$(dirname "$MEMDIR")"
    if [ -e "$MEMDIR" ] && [ ! -L "$MEMDIR" ]; then
      cp -an "$MEMDIR"/. "$STORE"/ 2>/dev/null || true
      rm -rf "$MEMDIR"
    fi
    ln -sfn "$STORE" "$MEMDIR"
  SHELL
end
