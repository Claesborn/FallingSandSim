{
  description = "Project5 flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs = { self, nixpkgs }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs { inherit system; };
  in {
    devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
            xmake
            gcc15
            llvm
            llvmPackages_20.clang-tools
            python314
            libGL
            pkg-config
            xorg.libX11
        ];

        # env = {
        #     OPENGL_opengl_LIBRARY = "/run/opengl-driver/lib/";
        #     OPENGL_INCLUDE_DIR = "/run/opengl-driver/include/";
        #     LD_LIBRARY_PATH = "/run/opengl-driver/lib:/run/opengl-driver-32/lib";
        # };
    };
  };
}
