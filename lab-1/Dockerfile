FROM nixos/nix
RUN echo >> /etc/nix/nix.conf
RUN echo "experimental-features = nix-command flakes" >> /etc/nix/nix.conf
WORKDIR /web
COPY . .
RUN nix develop --command build
