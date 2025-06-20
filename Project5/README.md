# Falling Sand Simulator

## Prerequisites:

The prerequisites only matter for non-Nix users. If you use Nix,
jump to the Nix-section below.

- Install [`xmake`](https://xmake.io/)

## Windows Development

### VS Code

Install the [xmake for VS Code](https://marketplace.visualstudio.com/items?itemName=tboox.xmake-vscode) extension.
And simply run:

```sh
xmake
```

### Visual Studio

Generate the project solution with the following command:

```sh
xmake project -k vsxmake -m "debug,release"
```

By opening the solution file, Visual Studio should call `xmake`
for compilation. This is because `xmake` may have custom build
scripts that need to run which Visual Studio doesn't support.

## Linux development

### Nix

Enter the development environment with:

```sh
nix develop
```

And compile with:

```sh
xmake
```

### Other distributions

Simply run the following to compile:

```sh
xmake
```
