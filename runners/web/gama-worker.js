import { wasi_snapshot_preview1 } from "./polyfill.js";

// Minimal WASI polyfill for the necessary imports
// begining

let p = {
  instance: undefined,
  module: undefined,
  canvas: undefined,
  ctx: undefined,
  running: true,
  state: 'unready',
  queue: [],
  init: { width: 500, height: 500, title: "gama app" },
  last_t: Date.now(),
  mouse: {
    x: 0, y: 0, mx: 0, my: 0,
    down: false,
    pressed: false,
  },
  keyboard: {
    down: [],
  },
};

// Create a proxy to catch any missing WASI functions
const gapi = {
  init: (width, height, title) => {
    p.init.width = width;
    p.init.height = height;
    p.init.title = takeString(title);
  },
  log: function(txt) {
    console.log(txt);
  },
  quit: () => {
    p.running = false;
    console.info("gama quit triggered");
  },
  draw_line: (x1, y1, x2, y2, size, r, b, g, a) => {
    p.queue.push({
      type: 'draw/line',
      start: [x1, y1],
      stop: [x2, y2],
      color: [r, g, b, a]
    });
  },
  draw_rect: (x, y, w, h, r, g, b, a) => {
    p.queue.push({
      type: 'draw/rect',
      pos: [x, y],
      size: [w, h],
      color: [r, g, b, a]
    });
  },
  draw_rounded_rect: (x, y, w, h, rad, r, g, b, a) => {
    p.queue.push({
      type: 'draw/roundrect',
      pos: [x, y],
      size: [w, h],
      radius: rad,
      color: [r, g, b, a]
    });
  },

  draw_circle: (x, y, rad, r, g, b, a) => {
    p.queue.push({
      type: 'draw/circle',
      pos: [x, y],
      radius: rad,
      color: [r, g, b, a],
    });
  },
  draw_triangle: (x1, y1, x2, y2, x3, y3, r, b, g, a) => {
    p.queue.push({
      type: 'draw/triangle',
      a: [x1, y1],
      b: [x2, y2],
      c: [x3, y3],
      color: [r, g, b, a]
    });
  },
  draw_text: (x, y, size, txt, font, style, r, g, b, a) => {
    p.queue.push({
      type: 'draw/text',
      pos: [x, y],
      text: takeString(txt),
      font: takeString(font),
      color: [r, g, b, a],
      size,
      style,
    });
  },
  set_bg_color: (r, g, b, a) => {
    p.queue.push({
      type: 'set/bg',
      color: [r, g, b, a],
    });
  },
  get_mouse_move: (x_ptr, y_ptr) => {
    setDoublePtr(x_ptr, p.mouse.mx);
    setDoublePtr(y_ptr, p.mouse.my);
  },
  mouse_get: (x_ptr, y_ptr) => {
    setDoublePtr(x_ptr, p.mouse.x);
    setDoublePtr(y_ptr, p.mouse.y);
    return 0;
  },
  mouse_down: () => p.mouse.down ? 1 : 0,
  mouse_pressed: () => p.mouse.pressed ? 1 : 0,
  yield: (dt_ptr) => {
    const now = Date.now();
    const dt = (now - p.last_t) / 1000;
    setDoublePtr(dt_ptr, dt);
    p.last_t = now;
    return p.running ? 1 : 0;
  },
  resize: (width, height) => {
    p.queue.push({
      type: 'resize',
      size: [width, height],
    });
  },
  fullscreen: full => {
    p.queue.push({
      type: 'fullscreen',
      full: full == 1,
    });
  },
  runs: () => p.running ? 1 : 0,
  key_pressed: (t, k) => {
    return p.keyboard.down.includes(String.fromCodePoint(t, k)) ? 1 : 0;
  },
  wait_queue: () => { }
};

function takeString(ptr) {
  const arr = new Uint8Array(p.instance.exports.memory.buffer);
  let end = ptr;
  while (arr[end] !== 0 && end - ptr < 100 && end < arr.length) {
    end++;
  }
  return new TextDecoder().decode(arr.slice(ptr, end));
}



self.onmessage = function(event) {
  if (p.state == 'unready') {
    // Combined import object with both WASI and GAPI imports
    const imports = {
      wasi_snapshot_preview1,
      env: gapi,  // In case it's also imported as 'env'
      gapi: gapi,
    };

    WebAssembly.instantiateStreaming(fetch(event.data), imports).then(({ instance, module }) => {
      p.instance = instance;
      p.module = module;
      console.log(instance);
      console.log('running gama_setup');
      const mode = instance.exports.gama_mode();

      if (mode != 2) {
        console.error("Gama program in invalid mode: ", mode);
        alert(`Gama module reports invalid mode ${mode}, mode should be == 2(setup mode)`);
      }

      const res = instance.exports.gama_setup();
      console.log('setup exited with status ', res, 'sending queue and init signal');
      postQueue();
      self.postMessage({
        type: "initialize",
        ...p.init,
      });
      p.state = 'ready';
      console.info("worker state switching to READY")
    }).catch(error => {
      console.error("Error instantiating WebAssembly module:", error);
    });
  } else if (p.state == 'ready') {
    if (event.data == null) {
      p.instance.exports.gama_loop();
      postQueue();
      self.postMessage(null);
      p.keyboard.down = [];
      p.mouse.pressed = false;
    } else {
      if (event.data.type == 'event/mousemove') {
        p.mouse.x = event.data.position[0];
        p.mouse.y = event.data.position[1];
        p.mouse.mx = event.data.movement[0];
        p.mouse.my = event.data.movement[1];
      } else if (event.data.type == 'event/mousedown') {
        p.mouse.down = true;
        p.mouse.pressed = true;
      } else if (event.data.type == 'event/mouseup') {
        p.mouse.down = false;

      } else if (event.data.type == 'event/keydown') {
        p.keyboard.down.push(event.data.key);
      } else if (event.data.type == 'event/keyup') {
        p.keyboard.down = p.keyboard.down.filter(k => k != event.data.key);
      }
    }
  } else {
    console.error("worker received unexpeced event: ", event, " while in state ", p.state);
  }
};
function postQueue() {
  for (const msg of p.queue) {
    self.postMessage(msg);
  }
  p.queue = [];
}

function setDoublePtr(ptr, val) {
  const float64View = new Float64Array(p.instance.exports.memory.buffer);

  // 4. Assign the value to the specific memory address
  // We divide by 8 because Float64Array indices are in 8-byte steps
  float64View[ptr / 8] = val;
}
