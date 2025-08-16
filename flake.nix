{
  description = "A Nix-flake-based C/C++ development environment";

  inputs = {
    nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0.1";
    flake-parts.url = "github:hercules-ci/flake-parts";
    devshell.url = "github:numtide/devshell";
  };

  outputs = inputs@{ flake-parts, ... }:
  # https://flake.parts/module-arguments.html
  flake-parts.lib.mkFlake { inherit inputs; } (top@{ config, withSystem, moduleWithSystem, ... }: {
    imports = [
      inputs.devshell.flakeModule
    ];
    systems = [
      "x86_64-linux"
    ];
    perSystem = { config, pkgs, ... }: {
      devshells.default = {
        packages = with pkgs; [
          clang-tools
          cmake
          cmake-language-server
          just
          (python3.withPackages (ps: [ ps.pytest ]))
          ruff
          gdb
          gcc
          samurai
        ];
      };
    };
  });
}

