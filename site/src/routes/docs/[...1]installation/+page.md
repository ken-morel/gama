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
    sudo apt update && sudo apt install tcc zig

    # For Arch Linux
    sudo pacman -Syu tcc zig
    ```

---

## The Gama Compiler Toolchain

Gama provides a seamless development experience by managing its own compiler toolchain. This eliminates the need for manual setup and lets you focus on building your game.

### Zero-Setup on Windows
On Windows, Gama comes bundled with pre-configured **TCC** and **Zig** compilers. There is no need to download any other tools or configure your `PATH` environment variables. Everything works out of the box.

### System-Native on Linux
On Linux, Gama respects the native ecosystem by relying on `tcc` and `zig` from your system's package manager. This ensures seamless integration and keeps the Gama installation lightweight.

### `gama dev` for Rapid Development
When you are actively developing, use the `gama dev` command. It runs your project using the incredibly fast **TCC** compiler for near-instantaneous builds. This command also enables **hot-reloading**, so your game will automatically restart whenever you save a file, providing a tight feedback loop for rapid iteration.

### `gama build` for Optimization
When you are ready to create a final, optimized release of your game, use the `gama build` command. This uses the more powerful **Zig CC** compiler, which performs more advanced optimizations to make your game run as fast as possible.

### Direct Compiler Access
For educational purposes or advanced build scripting, Gama provides direct proxy commands to its underlying compilers. This is a great way for students to learn how to use these tools without any complex setup. You can use the following commands as direct replacements for the real compiler commands:

-   `gama tcc [args...]`
-   `gama zig [args...]`
-   `gama zcc [args...]`
