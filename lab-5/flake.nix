{
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system: {
      devShells.default = (import nixpkgs { inherit system; }).mkShell { };
    });
}
