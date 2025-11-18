
let canvas = null;
let ctx = null;

// TextDecoder for converting C strings to JavaScript strings
const decoder = new TextDecoder();

// Helper to get the length of a null-terminated C string in Wasm memory
function get_string_length(ptr) {
  const view = new Uint8Array(memory.buffer);
  let len = 0;
  while (view[ptr + len] !== 0) {
    len++;
  }
  return len;
}

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
          gapi_log: (message_ptr) => {
            const message = decoder.decode(new Uint8Array(memory.buffer, message_ptr, get_string_length(message_ptr)));
            console.log(message);
          },
          gapi_set_title: (title_ptr) => {
            const title = decoder.decode(new Uint8Array(memory.buffer, title_ptr, get_string_length(title_ptr)));
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
          },
          gapi_canvas_clear: () => {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
          },
          gapi_canvas_clear_rect: (x, y, w, h) => {
            ctx.clearRect(x, y, w, h);
          },
          gapi_canvas_set_background: (r, g, b, a) => {
            ctx.fillStyle = `rgba(${r}, ${g}, ${b}, ${a / 255.0})`;
            ctx.fillRect(0, 0, canvas.width, canvas.height);
          },

          // --- Styling ---
          gapi_canvas_set_fill_color: (r, g, b, a) => {
            ctx.fillStyle = `rgba(${r}, ${g}, ${b}, ${a / 255.0})`;
          },
          gapi_canvas_set_stroke_color: (r, g, b, a) => {
            ctx.strokeStyle = `rgba(${r}, ${g}, ${b}, ${a / 255.0})`;
          },
          gapi_canvas_set_line_width: (width) => {
            ctx.lineWidth = width;
          },

          // --- Basic Shapes ---
          gapi_canvas_fill_rect: (x, y, w, h) => {
            ctx.fillRect(x, y, w, h);
          },
          gapi_canvas_stroke_rect: (x, y, w, h) => {
            ctx.strokeRect(x, y, w, h);
          },

          // --- Paths ---
          gapi_canvas_path_begin: () => {
            ctx.beginPath();
          },
          gapi_canvas_path_move_to: (x, y) => {
            ctx.moveTo(x, y);
          },
          gapi_canvas_path_line_to: (x, y) => {
            ctx.lineTo(x, y);
          },
          gapi_canvas_path_arc: (x, y, radius, start_angle, end_angle, counter_clockwise) => {
            ctx.arc(x, y, radius, start_angle, end_angle, counter_clockwise === 1);
          },
          gapi_canvas_path_close: () => {
            ctx.closePath();
          },
          gapi_canvas_path_fill: () => {
            ctx.fill();
          },
          gapi_canvas_path_stroke: () => {
            ctx.stroke();
          },

          // --- Transformations ---
          gapi_canvas_transform_save: () => {
            ctx.save();
          },
          gapi_canvas_transform_restore: () => {
            ctx.restore();
          },
          gapi_canvas_transform_translate: (x, y) => {
            ctx.translate(x, y);
          },
          gapi_canvas_transform_rotate: (angle_rad) => {
            ctx.rotate(angle_rad);
          },
          gapi_canvas_transform_scale: (x, y) => {
            ctx.scale(x, y);
          },
          gapi_canvas_transform_reset: () => {
            ctx.setTransform(1, 0, 0, 1, 0, 0);
          },

          // --- Text ---
          gapi_canvas_set_font: (font_ptr) => {
            const font = decoder.decode(new Uint8Array(memory.buffer, font_ptr, get_string_length(font_ptr)));
            ctx.font = font;
          },
          gapi_canvas_set_text_align: (align_ptr) => {
            const align = decoder.decode(new Uint8Array(memory.buffer, align_ptr, get_string_length(align_ptr)));
            ctx.textAlign = align;
          },
          gapi_canvas_fill_text: (text_ptr, x, y) => {
            const text = decoder.decode(new Uint8Array(memory.buffer, text_ptr, get_string_length(text_ptr)));
            ctx.fillText(text, x, y);
          },
          gapi_canvas_stroke_text: (text_ptr, x, y) => {
            const text = decoder.decode(new Uint8Array(memory.buffer, text_ptr, get_string_length(text_ptr)));
            ctx.strokeText(text, x, y);
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
      }
      break;
  }
};
