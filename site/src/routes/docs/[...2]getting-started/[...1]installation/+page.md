# Installation

Gama is designed for a simple, zero-setup experience.

## Installing on Windows

The easiest way to get started on Windows is to download the official installer.

1.  Navigate to the [**Gama Releases Page**](https://github.com/ken-morel/gama/releases/latest).
2.  Download the latest `gama-*-windows-setup.exe` file.
3.  Run the installer. It will automatically add `gama` to your system's PATH and bundle all the necessary compilers. **No other downloads are required.**

## Installing on Linux

On Linux, Gama integrates with your system's package manager.

1.  Go to the [**Gama Releases Page**](https://github.com/ken-morel/gama/releases/latest).
2.  Download the package for your distribution (`.deb` for Debian/Ubuntu, `.pkg.tar.zst` for Arch Linux).
3.  Install it using your system's package manager (e.g., `sudo dpkg -i gama.deb` or `sudo pacman -U gama.pkg.tar.zst`).
4.  **Install the dependencies**: Gama requires `tcc` and `zig`. You can install them with:
    ```bash
    # For Debian/Ubuntu
    sudo apt update && sudo apt install tcc

    # For Arch Linux
    sudo pacman -Syu tcc zig
    ```
    And you can download the latest version of `zig` from the [official website](https://ziglang.org/download/).
