export default class GamaInstance {
  constructor() {
    this.contexts = [];
    this.window = {
      x: 0,
      y: 0,
      side: 500,
    };
    this.initialized = false;
    this.maximized = false;
  }
  resize(width, height) {
    this.ctx.canvas.width = width;
    this.ctx.canvas.height = height;
    for (var context of this.contexts) {
      context.canvas.width = width;
      context.canvas.height = height;
    }
    this.window.side = Math.min(width, height);
    this.window.x = (width - this.window.side) / 2;
    this.window.y = (height - this.window.side) / 2;
  }
  maximize() {
    for (const ctx of this.contexts) {
      const resized = (e) => {
        if (!this.maximized) return;
        const rect = ctx.canvas.getBoundingClientRect();
        this.resize(rect.width, rect.height);
      };
      try {
        window.addEventListener('resize', resized);
      } catch (e) { }
      ctx.canvas.addEventListener('resize', resized);
      resized();
    }
  }
  bindKeyboard(elt) {
    elt.addEventListener('keydown', e => {
      console.log(getKey(e.key));
      this.worker.postMessage({
        type: 'event/keydown',
        key: getKey(e.key),
      });
    });
    elt.addEventListener('keyup', e => {
      this.worker.postMessage({
        type: 'event/keyup',
        key: getKey(e.key),
      });
    });
  }
  bind(canvas) {
    this.contexts.push(canvas.getContext('2d'));
    this.bindKeyboard(canvas);
    canvas.addEventListener('mousemove', e => {
      const r = e.target.getBoundingClientRect();
      this.worker.postMessage({
        type: 'event/mousemove',
        position: [...this._js_coord(e.clientX - r.x, e.clientY - r.y)]
      });
    });
    canvas.addEventListener('mousedown', e => {
      this.worker.postMessage({
        type: 'event/mousedown',
      });
    });
    canvas.addEventListener('mouseup', e => {
      this.worker.postMessage({
        type: 'event/mouseup',
      });
    });
    const touchpos = e => [...this._js_coord(e.touches[0].clientX, e.touches[0].clientY)];

    canvas.addEventListener('touchmove', e => {
      this.worker.postMessage({
        type: 'event/mousemove',
        position: touchpos(e)
      });
    });
    canvas.addEventListener('touchstart', e => {
      this.worker.postMessage({
        type: 'event/mousemove',
        position: touchpos(e)
      });
      this.worker.postMessage({
        type: 'event/mousedown',
      });
    });
    const handle = () => {
      this.worker.postMessage({
        type: 'event/mouseup',
      });
    };
    canvas.addEventListener('touchcancel', handle);
    canvas.addEventListener('touchend', handle);
  }
  async setup(wasmPath) {
    this.worker = new Worker(workerUrl, { type: 'module' });
    this.worker.onerror = this.handleWorkerError;

    const response = await fetch(wasmPath);
    const data = await response.arrayBuffer();

    this.worker.postMessage(data, [data]);

    return await new Promise(resolve => {
      this.worker.onmessage = (msg) => {
        this.handleWorkerMessage(msg);
        if (this.initialized) {
          resolve(msg.data);
          this.worker.onmessage = (msg) => this.handleWorkerMessage(msg);
        }
      }
    });
  }
  start() {
    this.yield();
  }
  destroy() {
    console.info("stopping worker");
    this.worker.terminate();
  }
  yield() {
    if (this.initialized) {
      requestAnimationFrame(() => {
        for (const context of this.contexts) {
          context.clearRect(0, 0, context.canvas.width, context.canvas.height);
          context.drawImage(this.ctx.canvas, 0, 0);
        }
        this.ctx.clearRect(0, 0, this.ctx.canvas.width, this.ctx.canvas.height);
        this.worker.postMessage(null);
      });
    } else console.error("Cannot call GamaInstance.yield because gama instance is not initialized(gm_init not called)");
  }
  handleWorkerCmd(d) {
    const ctx = this.ctx;

    switch (d.type) {
      case 'initialize':
        this.initialized = true;
        this.canv = new OffscreenCanvas(d.width, d.height);
        this.canv.width = d.width;
        this.canv.height = d.height;
        this.window.side = Math.min(d.width, d.height);
        this.window.x = (d.width - this.window.side) / 2;
        this.window.y = (d.height - this.window.side) / 2;
        this.applySize();
        this.ctx = this.canv.getContext('2d');
        try {
          document.querySelector('title').innerHTML = d.title;
        } catch (e) { }
        if (this.maximized) this.maximize();
        break;
      case 'draw/line':
        var { start, stop, color, size } = d;
        ctx.beginPath();
        ctx.moveTo(...this._c_coord(...start));
        ctx.lineTo(...this._c_coord(...stop));
        this._stroke(...color);
        ctx.lineWidth = this._c_one(size);
        ctx.stroke();
        break;
      case 'set/bg':
        var col = this._col(...d.color);
        for (var context of this.contexts) {
          context.canvas.style.backgroundColor = col;
        }
        break;
      case 'draw/rect':
        var { pos, size, color } = d;
        this._fill(...color);
        var [x, y] = this._c_coord(...pos);
        var [w, h] = [this._c_one(size[0]), this._c_one(size[1])];
        ctx.fillRect(x - w / 2, y - h / 2, w, h);
        break;
      case 'draw/roundrect':
        var { pos, size, radius, color } = d;
        var [w, h] = [this._c_one(size[0]), this._c_one(size[1])];
        var [topX, topY] = this._c_coord(...pos);
        topX -= w / 2; topY -= h / 2; // Center it
        var r = this._c_one(radius);
        if (w < 2 * r) r = w / 2;
        if (h < 2 * r) r = h / 2;
        this._fill(...color);
        ctx.beginPath();
        ctx.moveTo(topX + r, topY);
        ctx.arcTo(topX + w, topY, topX + w, topY + h, r);
        ctx.arcTo(topX + w, topY + h, topX, topY + h, r);
        ctx.arcTo(topX, topY + h, topX, topY, r);
        ctx.arcTo(topX, topY, topX + w, topY, r);
        ctx.closePath();
        ctx.fill();
        break;
      case 'draw/triangle':
        var { a, b, c, color } = d;

        this._fill(...color);

        ctx.beginPath();
        ctx.moveTo(...this._c_coord(...a));
        ctx.lineTo(...this._c_coord(...b));
        ctx.lineTo(...this._c_coord(...c));
        ctx.closePath();
        ctx.fill();
        break;

      case 'draw/circle':
        var { pos, radius, color } = d;
        ctx.beginPath();
        ctx.arc(...this._c_coord(...pos), this._c_one(radius), 0, 2 * Math.PI);
        this._fill(...color);
        this.ctx.fill();
        break;
      case 'draw/text':
        var { pos, text, style, size, font, color } = d;
        ctx.font = this._c_one(size).toFixed(0) + "px '" + font + "'";
        this._fill(...color);
        ctx.textAlign = 'center';
        ctx.textBaseline = 'middle';
        ctx.fillText(text, ...this._c_coord(...pos));
        break;
      case 'resize':
        var [width, height] = d.size;
        this.resize(...d.size)

        break;
      case 'fullscreen':
        //TODO: find the currently viewed bound canvas
        this.maximized = d.full == 1;
        if (d.full && this.initialized) this.maximize();
        break;
    }
  }
  handleWorkerMessage(event) {
    const d = event.data;
    if (d == null) { // loop completed, save and wait for next frame
      this.yield();
      return;
    } else if (d.type == 'multiple') {
      for (const cmd of d.commands) {
        this.handleWorkerCmd(cmd)
      }
    } else {
      this.handleWorkerCmd(d);
    }

  }
  applySize() {
    for (const context of this.contexts) {
      context.canvas.width = this.canv.width;
      context.canvas.height = this.canv.height;
    }
  }
  _col(r, g, b, a) {
    return `rgba(${r}, ${g}, ${b}, ${a})`;
  }
  _stroke(...col) {
    this.ctx.strokeStyle = this._col(...col);
  }
  _fill(...col) {
    this.ctx.fillStyle = this._col(...col);
  }
  handleWorkerError(error) {
    console.error('Worker error:', error);
  }


  _c_coord(x, y) {
    let norm_x = (x + 1.0) * 0.5
    let norm_y = (1.0 - y) * 0.5 // Invert Y-axis for screen coordinates

    return [norm_x * this.window.side + this.window.x, norm_y * this.window.side +
      this.window.y];
  }
  _js_offset(x, y) {
    return [x + this.window.x, y + this.window.y];
  }
  _js_unoffset(x, y) {
    return [x - this.window.x, y - this.window.y];
  }

  _js_coord(x, y) {
    let norm_x = (x - this.window.x) / this.window.side
    let norm_y = (y - this.window.y) / this.window.side

    return [(norm_x * 2) - 1.0, 1.0 - (norm_y * 2)]
  }

  _c_one(v) {
    return v * this.window.side * 0.5
  }

  _js_one(v) {
    return (v * 2) / this.window.side;
  }

  _c_rect(x, y, w, h) {
    let gx, gy = this._c_coord(x, y)

    let gw = w * 0.5 * this.window.side
    let gh = h * 0.5 * this.window.side

    return [gx - gw / 2, gy - gh / 2, gw, gh];
  }
}


const workerfn = () => {
  class GamaWASI {
    constructor(p) {
      this.p = p; // Reference to your state object (must contain p.instance)
    }

    get importObject() {
      const s = this;
      const mem = () => s.p.instance.exports.memory.buffer;
      const view = () => new DataView(mem());

      return {
        // --- Process & Environment ---
        proc_exit: (code) => console.log(`Process exited: ${code}`),
        sched_yield: () => 0,
        environ_sizes_get: (conf, bufsize) => {
          view().setUint32(conf, 0, true);
          view().setUint32(bufsize, 0, true);
          return 0;
        },
        environ_get: (environ, environ_buf) => 0,
        args_sizes_get: (argc, argv_buf_size) => {
          view().setUint32(argc, 0, true);
          view().setUint32(argv_buf_size, 0, true);
          return 0;
        },
        args_get: (argv, argv_buf) => 0,

        // --- Clock ---
        clock_time_get: (id, precision, ptr) => {
          const now = BigInt(Date.now()) * 1000000n;
          view().setBigUint64(ptr, now, true);
          return 0;
        },

        // --- Random ---
        random_get: (buf, len) => {
          crypto.getRandomValues(new Uint8Array(mem(), buf, len));
          return 0;
        },

        // --- File Descriptors (The meat of the logic) ---
        fd_write: (fd, iovs, iovs_len, nwritten) => {
          let total = 0;
          for (let i = 0; i < iovs_len; i++) {
            const ptr = view().getUint32(iovs + i * 8, true);
            const len = view().getUint32(iovs + i * 8 + 4, true);
            const txt = new TextDecoder().decode(new Uint8Array(mem(), ptr, len));
            fd === 1 ? console.log(txt) : console.warn(txt);
            total += len;
          }
          view().setUint32(nwritten, total, true);
          return 0;
        },

        fd_pwrite: (fd, iovs, iovs_len, offset, nwritten) => {
          // Offset is ignored here as we are writing to console
          return this.importObject.fd_write(fd, iovs, iovs_len, nwritten);
        },

        fd_read: () => 0,
        fd_pread: () => 0,
        fd_close: () => 0,
        fd_seek: () => 28, // ENOTSUP
        fd_tell: () => 28,
        fd_sync: () => 0,
        fd_datasync: () => 0,
        fd_advise: () => 0,
        fd_allocate: () => 28,

        fd_fdstat_get: (fd, buf) => {
          const v = view();
          v.setUint8(buf, 1); // Filetype: Character Device
          v.setUint16(buf + 2, 0, true); // Flags
          v.setBigUint64(buf + 8, 0n, true); // Rights base
          v.setBigUint64(buf + 16, 0n, true); // Rights inheriting
          return 0;
        },

        fd_fdstat_set_flags: () => 0,
        fd_filestat_get: () => 28,
        fd_filestat_set_size: () => 28,
        fd_filestat_set_times: () => 28,
        fd_prestat_get: () => 8, // EBADF (No preopened dirs)
        fd_prestat_dir_name: () => 8,
        fd_readdir: () => 28,
        fd_renumber: () => 28,

        // --- Path Operations ---
        path_open: () => 44, // ENOENT
        path_create_directory: () => 28,
        path_filestat_get: () => 44,
        path_filestat_set_times: () => 28,
        path_link: () => 28,
        path_readlink: () => 44,
        path_remove_directory: () => 28,
        path_rename: () => 28,
        path_symlink: () => 28,
        path_unlink_file: () => 28,

        // --- Networking ---
        poll_oneoff: () => 28,
        sock_recv: () => 28,
        sock_send: () => 28,
        sock_shutdown: () => 28,
      };
    }
  }
  const wasi_snapshot_preview1 = new GamaWASI().importObject;


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
      x: 0, y: 0,
      down: false,
    },
    keyboard: {
      down: [],
    },
  };
  try {
    window.p = p;
  } catch (e) { } //DEBUG: remove this

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
    mouse_get: (x_ptr, y_ptr) => {
      setDoublePtr(x_ptr, p.mouse.x);
      setDoublePtr(y_ptr, p.mouse.y);
      return 0;
    },
    mouse_down: () => p.mouse.down ? 1 : 0,
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
    key_down: (t, k) => {
      return p.keyboard.down.includes(String.fromCodePoint(t, k)) ? 1 : 0;
    },
    wait_queue: () => { }
  };

  const utf8Decoder = new TextDecoder("utf-8");

  function takeString(ptr) {
    if (!ptr || ptr === 0) return "";

    const buffer = p.instance.exports.memory.buffer;
    const view = new Uint8Array(buffer);

    let end = ptr;
    const maxSearch = 1024;
    while (view[end] !== 0 && (end - ptr) < maxSearch && end < view.length) {
      end++;
    }

    const bytes = view.slice(ptr, end);

    return utf8Decoder.decode(bytes);
  }

  self.onmessage = function(event) {
    if (p.state == 'unready') {
      const imports = {
        wasi_snapshot_preview1,
        env: gapi,  // In case it's also imported as 'env'
        gapi: gapi,
      };

      WebAssembly.instantiate(event.data, imports).then(({ instance, module }) => {
        p.instance = instance;
        p.module = module;
        console.log(instance);
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
        } else if (event.data.type == 'event/mousedown') {
          p.mouse.down = true;
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
    self.postMessage({
      type: 'multiple',
      commands: p.queue,
    });
    p.queue = [];
  }

  function setDoublePtr(ptr, val) {
    const mem = p.instance.exports.memory.buffer;
    // Safety check for alignment
    if (ptr % 8 === 0) {
      new Float64Array(mem)[ptr / 8] = val;
    } else {
      new DataView(mem).setFloat64(ptr, val, true);
    }
  }
};

export const workerUrl = URL.createObjectURL(new Blob([`(${workerfn.toString()})()`], { type: 'text/javascript' }));

export function getKey(key) {
  return KEYS[key] || '  ';
}
export const KEYS = {
  // Direction keys -> 'a' class
  "ArrowUp": "au",
  "ArrowDown": "ad",
  "ArrowLeft": "al",
  "ArrowRight": "ar",

  // Special keys -> 's' class
  "Escape": "sx",
  "Insert": "si",
  "Delete": "sd",
  "Backspace": "sb",
  "Tab": "st",
  "Enter": "sn",

  // Modifiers -> 'm' class
  "Shift": "ms",
  "Control": "mc",
  "Alt": "ma",
  "Meta": "mS", // Windows/Command key
  "CapsLock": "mC",

  // Characters (Lowercase) -> 'c' class
  "a": "ca", "b": "cb", "c": "cc", "d": "cd", "e": "ce", "f": "cf",
  "g": "cg", "h": "ch", "i": "ci", "j": "cj", "k": "ck", "l": "cl",
  "m": "cm", "n": "cn", "o": "co", "p": "cp", "q": "cq", "r": "cr",
  "s": "cs", "t": "ct", "u": "cu", "v": "cv", "w": "cw", "x": "cx",
  "y": "cy", "z": "cz",

  // Characters (Uppercase - mapping to same codes)
  "A": "ca", "B": "cb", "C": "cc", "D": "cd", "E": "ce", "F": "cf",
  "G": "cg", "H": "ch", "I": "ci", "J": "cj", "K": "ck", "L": "cl",
  "M": "cm", "N": "cn", "O": "co", "P": "cp", "Q": "cq", "R": "cr",
  "S": "cs", "T": "ct", "U": "cu", "V": "cv", "W": "cw", "X": "cx",
  "Y": "cy", "Z": "cz",

  // Digits
  "0": "c0", "1": "c1", "2": "c2", "3": "c3", "4": "c4",
  "5": "c5", "6": "c6", "7": "c7", "8": "c8", "9": "c9",

  // Whitespace and punctuation
  " ": "c ",
  ",": "c,",
  ".": "c.",
  "/": "c/",
  ";": "c;",
  "'": "c'",
  "\\": "c\\",
  "[": "c[",
  "]": "c]",
  "-": "c-",
  "=": "c=",

  // Function keys -> 'f' class
  "F1": "f1",
  "F2": "f2",
  "F3": "f3",
  "F4": "f4",
  "F5": "f5",
  "F6": "f6",
  "F7": "f7",
  "F8": "f8",
  "F9": "f9",
  "F10": "fa",
  "F11": "fb",
  "F12": "fc"
};
