{
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = import nixpkgs { inherit system; }; in
      {
        devShells.default = pkgs.mkShell {
          packages = [
            (pkgs.rWrapper.override {
              packages = [
                pkgs.rPackages.readr
                pkgs.rPackages.rmarkdown
              ];
            })
            pkgs.pandoc
            (pkgs.writeShellScriptBin "build" ''
              Rscript -e 'rmarkdown::render("lab-1.Rmd")'
            '')
          ];
        };
      });
  inputs.flake-utils.url = "github:numtide/flake-utils";
}
