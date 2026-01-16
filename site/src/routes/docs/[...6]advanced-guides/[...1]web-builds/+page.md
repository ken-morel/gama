# How to Build for the Web

Gama makes it simple to compile your C project into WebAssembly, allowing it to run in any modern web browser.

## The Web Build Command

To build your project for the web, navigate to your project's root directory and run the `build` command with the `web` (or `w`) flag:

```bash
gama build web

# build and run
gama build web -r
```

This command does the following:
1.  Compiles your C source code into a WebAssembly file (`{projectname}.wasm`).
2.  Uses the Zig C compiler (`gama zcc`) for this process, as it has excellent support for WASM targets.
3.  Creates a `build/web/` directory in your project.
4.  Populates this directory with all the necessary files to run your game:
    -   `index.html`: The main HTML page to load your game.
    -   `gama.js`: The JavaScript "runner" that handles rendering and browser integration.
    -   `main.wasm`: Your compiled game logic.



## Running Your Web Build

You can simply use:
```bash
gama run web
```

Due to security reasons you cannot run your app simply by opening the `index.html`
localy in your browser, similarly, before your browser can succesfully run
the gama application, the server must be configured specially. TO learn more
you can view the [`gama.js` guide](/docs/advanced-guides/gama.js)

It also starts a local server running your project, you can access it at
`http://localhost:8095` default address and with proper configuration.
