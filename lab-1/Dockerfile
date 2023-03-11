FROM nixos/nix
RUN echo "\nexperimental-features = nix-command flakes" >> /etc/nix/nix.conf
WORKDIR /web
COPY . .
RUN nix shell nixpkgs#hello -c hello
