// buildtemplate/gama.js

class Gama {
    constructor(wasmModule) {
        this.wasmModule = wasmModule;
        this.worker = null;
        this.canvas = null;
        this.workerInitialized = false;
    }

    static async load(wasmUrl) {
        // Fetch and compile the Wasm module once.
        // This module can then be transferred to a worker.
        const response = await fetch(wasmUrl);
        const wasmModule = await WebAssembly.compile(await response.arrayBuffer());
        return new Gama(wasmModule);
    }

    attachCanvas(canvasElement) {
        if (!canvasElement.transferControlToOffscreen) {
            throw new Error("Your browser does not support OffscreenCanvas. This Gama instance cannot run.");
        }

        this.canvas = canvasElement;

        // Create the worker from the dedicated worker script.
        this.worker = new Worker('gama.worker.js');

        // Handle messages from the worker
        this.worker.onmessage = (event) => {
            switch (event.data.type) {
                case "setTitle":
                    document.title = event.data.title;
                    break;
                // Add other message handlers here if needed
            }
        };

        // Function to update canvas dimensions and inform the worker
        const setCanvasDimensions = () => {
            const width = window.innerWidth;
            const height = window.innerHeight;

            // Set the visible canvas attributes
            this.canvas.width = width;
            this.canvas.height = height;

            // If the worker is ready, send it the new dimensions
            if (this.workerInitialized) {
                this.worker.postMessage({ type: 'resize', width: width, height: height });
            }
        };

        // Initial setup of canvas dimensions
        setCanvasDimensions();

        // Transfer canvas control and compiled Wasm module to the worker
        const offscreen = this.canvas.transferControlToOffscreen();
        this.worker.postMessage({
            type: 'init',
            canvas: offscreen,
            wasmModule: this.wasmModule, // Transfer the compiled Wasm module
            width: this.canvas.width,
            height: this.canvas.height
        }, [offscreen, this.wasmModule]); // Mark as transferable objects

        // Mark worker as initialized after sending the initial message
        this.workerInitialized = true;

        // Listen for window resize events to update canvas dimensions
        window.addEventListener('resize', setCanvasDimensions);

        return this;
    }
}
