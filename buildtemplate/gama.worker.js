
let canvas = null;
let ctx = null;

self.onmessage = async (event) => {
  const sync_buffer = new SharedArrayBuffer(4);
  const sync_flag = new Int32Array(sync_buffer);

  // We are allocating 1GB here. 1 page = 64KiB. 1GB = 16,384 pages.
  const memory = new WebAssembly.Memory({
    initial: 16384,
    maximum: 16384,
    shared: true
  });

  switch (event.data.type) {
    case 'init':
      canvas = event.data.canvas;
      ctx = canvas.getContext('2d');
      // Set initial size
      canvas.width = event.data.width;
      canvas.height = event.data.height;

      const importObject = {
        env: {
          // Provide the shared memory to the Wasm module.
          memory: memory,

          gapi_init: () => {
            return 0;
          },
          gapi_log: console.log,
          gapi_set_title: (title) => {
            self.postMessage({ type: 'setTitle', title: title });
          },
          gapi_yield: () => {
            Atomics.store(sync_flag, 0, 0);

            requestAnimationFrame(() => {
              Atomics.store(sync_flag, 0, 1);
              Atomics.notify(sync_flag, 0, 1);
            });

            Atomics.wait(sync_flag, 0, 0);
          },
          gapi_canvas_set_size: (width, height) => {
            if (canvas) {
              canvas.width = width;
              canvas.height = height;
            }
          }
        }
      };

      try {
        const { instance, module } = await WebAssembly.instantiateStreaming(fetch('./game.wasm'), importObject);
   

        // Now, start the C main() function, which contains the while(1) loop.
        const result = instance.exports.main();
        console.info("Program exited with code", result);

      } catch (error) {
        console.error("Error loading WebAssembly module:", error);
      }
      break;

    case 'resize':
      if (canvas) {
        canvas.width = event.data.width;
        canvas.height = event.data.height;
        // If your C code needs to be explicitly notified of a resize,
        // you would call a Wasm-exported function here.
        // For now, we just update the OffscreenCanvas dimensions.
      }
      break;
  }
};
