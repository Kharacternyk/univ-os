{
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = import nixpkgs { inherit system; }; in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          name = "static-site";
          src = ./.;
          buildInputs = [
            (pkgs.rWrapper.override {
              packages = [
                pkgs.rPackages.readr
                pkgs.rPackages.rmarkdown
              ];
            })
            pkgs.pandoc
          ];
          buildPhase = ''
            Rscript -e 'rmarkdown::render("lab-1.Rmd")'
          '';
          installPhase = ''
            mkdir -p $out
            cp lab-1.html $out/index.html
          '';
        };
      });

  inputs.flake-utils.url = "github:numtide/flake-utils";
}
