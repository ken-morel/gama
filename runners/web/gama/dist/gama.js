// src/color.ts
function gmcDecode(col) {
  col = col >>> 0;
  return {
    r: col >>> 24 & 255,
    g: col >>> 16 & 255,
    b: col >>> 8 & 255,
    a: col & 255
  };
}
function gmcToCss(c) {
  const { r, g, b, a } = gmcDecode(c);
  const normalizedAlpha = (a / 255).toFixed(2);
  return `rgba(${r}, ${g}, ${b}, ${normalizedAlpha})`;
}

// src/keyboard.ts
function getKeyCode(key) {
  return KEYS[key] || "  ";
}
var KEYS = {
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
  "Meta": "mS",
  // Windows/Command key
  "CapsLock": "mC",
  // Characters (Lowercase) -> 'c' class
  "a": "ca",
  "b": "cb",
  "c": "cc",
  "d": "cd",
  "e": "ce",
  "f": "cf",
  "g": "cg",
  "h": "ch",
  "i": "ci",
  "j": "cj",
  "k": "ck",
  "l": "cl",
  "m": "cm",
  "n": "cn",
  "o": "co",
  "p": "cp",
  "q": "cq",
  "r": "cr",
  "s": "cs",
  "t": "ct",
  "u": "cu",
  "v": "cv",
  "w": "cw",
  "x": "cx",
  "y": "cy",
  "z": "cz",
  // Characters (Uppercase - mapping to same codes)
  "A": "ca",
  "B": "cb",
  "C": "cc",
  "D": "cd",
  "E": "ce",
  "F": "cf",
  "G": "cg",
  "H": "ch",
  "I": "ci",
  "J": "cj",
  "K": "ck",
  "L": "cl",
  "M": "cm",
  "N": "cn",
  "O": "co",
  "P": "cp",
  "Q": "cq",
  "R": "cr",
  "S": "cs",
  "T": "ct",
  "U": "cu",
  "V": "cv",
  "W": "cw",
  "X": "cx",
  "Y": "cy",
  "Z": "cz",
  // Digits
  "0": "c0",
  "1": "c1",
  "2": "c2",
  "3": "c3",
  "4": "c4",
  "5": "c5",
  "6": "c6",
  "7": "c7",
  "8": "c8",
  "9": "c9",
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

// src/sab.ts
function writeYieldResult(buf, offset, res) {
  const view = new DataView(buf);
  view.setFloat32(offset, res.mouse.x, true);
  offset += 4;
  view.setFloat32(offset, res.mouse.y, true);
  offset += 4;
  view.setUint8(offset, res.mouse.down ? 1 : 0);
  offset += 1;
  const keyCount = res.keyboard.down.length;
  view.setUint8(offset, keyCount);
  offset += 1;
  for (const key of res.keyboard.down) {
    view.setUint8(offset, key.charCodeAt(0));
    view.setUint8(offset, key.charCodeAt(1));
    offset += 2;
  }
}

// src/index.ts
var WORKER_URL = URL.createObjectURL(new Blob(['"use strict";\n(() => {\n  var __defProp = Object.defineProperty;\n  var __defNormalProp = (obj, key, value) => key in obj ? __defProp(obj, key, { enumerable: true, configurable: true, writable: true, value }) : obj[key] = value;\n  var __publicField = (obj, key, value) => __defNormalProp(obj, typeof key !== "symbol" ? key + "" : key, value);\n\n  // ../wasi.ts\n  var GamaWASI = class {\n    constructor() {\n      __publicField(this, "instance", null);\n    }\n    setInstance(inst) {\n      this.instance = inst;\n    }\n    get importObject() {\n      const s = this;\n      const mem = () => this.instance.exports.memory.buffer;\n      const view = () => new DataView(mem());\n      return {\n        // --- Process & Environment ---\n        proc_exit: (code) => console.log(`Process exited: ${code}`),\n        sched_yield: () => 0,\n        environ_sizes_get: (conf, bufsize) => {\n          view().setUint32(conf, 0, true);\n          view().setUint32(bufsize, 0, true);\n          return 0;\n        },\n        environ_get: (environ, environ_buf) => 0,\n        args_sizes_get: (argc, argv_buf_size) => {\n          view().setUint32(argc, 0, true);\n          view().setUint32(argv_buf_size, 0, true);\n          return 0;\n        },\n        args_get: (argv, argv_buf) => 0,\n        // --- Clock ---\n        clock_time_get: (id, precision, ptr) => {\n          const now = BigInt(Date.now()) * 1000000n;\n          view().setBigUint64(ptr, now, true);\n          return 0;\n        },\n        // --- Random ---\n        random_get: (buf, len) => {\n          crypto.getRandomValues(new Uint8Array(mem(), buf, len));\n          return 0;\n        },\n        // --- File Descriptors (The meat of the logic) ---\n        fd_write: (fd, iovs, iovs_len, nwritten) => {\n          let total = 0;\n          for (let i = 0; i < iovs_len; i++) {\n            const ptr = view().getUint32(iovs + i * 8, true);\n            const len = view().getUint32(iovs + i * 8 + 4, true);\n            const txt = new TextDecoder().decode(new Uint8Array(mem(), ptr, len));\n            fd === 1 ? console.log(txt) : console.warn(txt);\n            total += len;\n          }\n          view().setUint32(nwritten, total, true);\n          return 0;\n        },\n        fd_pwrite: (fd, iovs, iovs_len, offset, nwritten) => {\n          return this.importObject.fd_write(fd, iovs, iovs_len, nwritten);\n        },\n        fd_read: () => 0,\n        fd_pread: () => 0,\n        fd_close: () => 0,\n        fd_seek: () => 28,\n        // ENOTSUP\n        fd_tell: () => 28,\n        fd_sync: () => 0,\n        fd_datasync: () => 0,\n        fd_advise: () => 0,\n        fd_allocate: () => 28,\n        fd_fdstat_get: (fd, buf) => {\n          const v = view();\n          v.setUint8(buf, 1);\n          v.setUint16(buf + 2, 0, true);\n          v.setBigUint64(buf + 8, 0n, true);\n          v.setBigUint64(buf + 16, 0n, true);\n          return 0;\n        },\n        fd_fdstat_set_flags: () => 0,\n        fd_filestat_get: () => 28,\n        fd_filestat_set_size: () => 28,\n        fd_filestat_set_times: () => 28,\n        fd_prestat_get: () => 8,\n        // EBADF (No preopened dirs)\n        fd_prestat_dir_name: () => 8,\n        fd_readdir: () => 28,\n        fd_renumber: () => 28,\n        // --- Path Operations ---\n        path_open: () => 44,\n        // ENOENT\n        path_create_directory: () => 28,\n        path_filestat_get: () => 44,\n        path_filestat_set_times: () => 28,\n        path_link: () => 28,\n        path_readlink: () => 44,\n        path_remove_directory: () => 28,\n        path_rename: () => 28,\n        path_symlink: () => 28,\n        path_unlink_file: () => 28,\n        // --- Networking ---\n        poll_oneoff: () => 28,\n        sock_recv: () => 28,\n        sock_send: () => 28,\n        sock_shutdown: () => 28\n      };\n    }\n  };\n\n  // src/wasm-utils.ts\n  var utf8Decoder = new TextDecoder("utf-8");\n  function takeString(mem, ptr) {\n    if (!ptr || ptr === 0) return "";\n    const buffer = mem.buffer;\n    const view = new Uint8Array(buffer);\n    let end = ptr;\n    const maxSearch = 1024;\n    while (view[end] !== 0 && end - ptr < maxSearch && end < view.length) {\n      end++;\n    }\n    const bytes = view.slice(ptr, end);\n    return utf8Decoder.decode(bytes);\n  }\n  function setDoublePtr(mem, ptr, val) {\n    if (ptr % 8 === 0) {\n      new Float64Array(mem.buffer)[ptr / 8] = val;\n    } else {\n      new DataView(mem.buffer).setFloat64(ptr, val, true);\n    }\n  }\n\n  // src/keyboard.ts\n  var keyCode = (t, k) => String.fromCodePoint(t, k);\n\n  // src/sab.ts\n  function readYieldResult(buf, offset) {\n    const view = new DataView(buf);\n    const mouse = {\n      x: view.getFloat32(offset, true),\n      y: view.getFloat32(offset + 4, true),\n      down: view.getUint8(offset + 8) !== 0\n    };\n    offset += 9;\n    const keyCount = view.getUint8(offset);\n    offset += 1;\n    const end = offset + keyCount * 2;\n    const downKeys = [];\n    while (offset < end)\n      downKeys.push(keyCode(view.getUint8(offset++), view.getUint8(offset++)));\n    return {\n      mouse,\n      keyboard: { down: downKeys }\n    };\n  }\n\n  // src/worker.ts\n  var d = { mod: null, inst: null, buff: null, cmds: [], size: [500, 500], mem: null, running: true, mouse: { x: 0, y: 0, down: false }, last_t: 0, keyboard: { down: [] }, image_counter: 1, buff32: null };\n  var state = 1;\n  function runGama() {\n    console.info("Running gama.main");\n    const ret = d.inst.exports.gama_run();\n    if (ret == 0)\n      console.info("main function returned 0");\n    else\n      console.error(`main function returned ${ret}`);\n  }\n  self.onmessage = (msg) => {\n    if (state == 1) {\n      state = 0;\n      let data = msg.data;\n      const wasi = new GamaWASI();\n      const wasmImports = { wasi_snapshot_preview1: wasi.importObject, gapi };\n      WebAssembly.instantiate(data.wasmData, wasmImports).then(function({ module: mod, instance: inst }) {\n        d.mod = mod;\n        d.inst = inst;\n        d.mem = d.inst.exports.memory;\n        wasi.setInstance(inst);\n        state = 2;\n        self.postMessage({\n          ok: true,\n          error: null\n        });\n      });\n    } else if (state == 2) {\n      state = 3;\n      let data = msg.data;\n      d.buff = data.buffer;\n      d.buff32 = new Int32Array(d.buff);\n      setTimeout(runGama, 0);\n    }\n  };\n  var gapi = {\n    init: (width, height, title) => {\n      self.postMessage({\n        type: "resize",\n        size: [width, height]\n      });\n      const txt = takeString(d.mem, title);\n      self.postMessage({\n        type: "set-title",\n        title: txt\n      });\n      d.last_t = Date.now();\n      console.info(`gm_init called, with dimensions ${width}x${height} and title: \\`${txt}\\``);\n    },\n    log: function(txt) {\n      console.log(takeString(d.mem, txt));\n    },\n    quit: () => {\n      d.running = false;\n      console.info("gm_quit called");\n    },\n    draw_line: (x1, y1, x2, y2, size, c) => {\n      d.cmds.push(["line", x1, y1, x2, y2, size, c]);\n    },\n    draw_rect: (x, y, w, h, c) => {\n      d.cmds.push(["rect", x, y, w, h, c]);\n    },\n    draw_rounded_rect: (x, y, w, h, rad, col) => {\n      d.cmds.push([\n        "roundrect",\n        x,\n        y,\n        w,\n        h,\n        rad,\n        col\n      ]);\n    },\n    draw_circle: (x, y, rad, col) => {\n      d.cmds.push([\n        "circle",\n        x,\n        y,\n        rad,\n        col\n      ]);\n    },\n    draw_triangle: (x1, y1, x2, y2, x3, y3, col) => {\n      d.cmds.push([\n        "triangle",\n        x1,\n        y1,\n        x2,\n        y2,\n        x3,\n        y3,\n        col\n      ]);\n    },\n    draw_text: (x, y, size, txt, font, style, col) => {\n      d.cmds.push([\n        "text",\n        x,\n        y,\n        size,\n        takeString(d.mem, txt),\n        takeString(d.mem, font),\n        style,\n        col\n      ]);\n    },\n    set_background_color: (col) => {\n      self.postMessage({\n        type: "set-background-color",\n        color: col\n      });\n    },\n    mouse_get: (x_ptr, y_ptr) => {\n      setDoublePtr(d.mem, x_ptr, d.mouse.x);\n      setDoublePtr(d.mem, y_ptr, d.mouse.y);\n      return 0;\n    },\n    mouse_down: () => d.mouse.down ? 1 : 0,\n    resize: (width, height) => {\n      self.postMessage({ type: "resize", size: [width, height] });\n    },\n    fullscreen: (full) => {\n      self.postMessage({ type: "fullscreen", fullscreen: full != 0 });\n    },\n    runs: () => d.running ? 1 : 0,\n    key_down: (t, k) => {\n      return d.keyboard.down.includes(String.fromCodePoint(t, k)) ? 1 : 0;\n    },\n    create_image: (data_ptr, width, height) => {\n      if (data_ptr * width * height == 0) return 1;\n      const buffer = d.inst.exports.memory.buffer;\n      const view = new Uint8Array(buffer);\n      const size = width * height * 4;\n      const bytes = view.slice(data_ptr, data_ptr + size);\n      const id = d.image_counter++;\n      const data = new Uint8ClampedArray(size);\n      for (let i = 0; i < size; i++)\n        data[i] = bytes[i];\n      self.postMessage({ type: "create-image", id, data, size: [width, height] });\n      return id;\n    },\n    draw_image: (handle, x, y, width, height) => {\n      d.cmds.push([\n        "image",\n        handle,\n        x,\n        y,\n        width,\n        height\n      ]);\n    },\n    draw_image_part: (handle, sx, sy, sw, sh, x, y, w, h) => {\n      d.cmds.push([\n        "image-part",\n        handle,\n        sx,\n        sy,\n        sw,\n        sh,\n        x,\n        y,\n        w,\n        h\n      ]);\n    },\n    yield: (dt_ptr) => {\n      self.postMessage({\n        type: "draw",\n        cmds: d.cmds\n      });\n      d.cmds = [];\n      Atomics.store(d.buff32, 0, 0);\n      self.postMessage({\n        type: "yield"\n      });\n      Atomics.wait(d.buff32, 0, 0);\n      const data = readYieldResult(d.buff, 1);\n      d.mouse = data.mouse;\n      d.keyboard = data.keyboard;\n      const now = Date.now();\n      const dt = (now - d.last_t) / 1e3;\n      setDoublePtr(d.mem, dt_ptr, dt);\n      d.last_t = now;\n      return d.running ? 1 : 0;\n    }\n  };\n})();\n'], {
  type: "application/javascript"
}));
var Gama = class _Gama {
  constructor(w) {
    this.window = {
      side: 500,
      offset: { x: 0, y: 0 }
    };
    this.output = null;
    this.yielding = {
      keyboard: {
        down: []
      },
      mouse: {
        down: false,
        x: 0,
        y: 0
      }
    };
    this.sizemode = "natural";
    this.worker = w;
    this.canvas = {
      front: new OffscreenCanvas(500, 500),
      back: new OffscreenCanvas(500, 500)
    };
    this.ctx = {
      front: this.canvas.front.getContext("2d"),
      back: this.canvas.back.getContext("2d")
    };
    this.buffer = new SharedArrayBuffer(1024);
    this.buffer32 = new Int32Array(this.buffer);
    this.worker.onerror = this.workerError;
    this.worker.onmessage = null;
  }
  static create(wasmPath) {
    return new Promise(async function(resolve, reject) {
      const fetchResponse = await fetch(wasmPath);
      const wasmDataBuffer = await fetchResponse.arrayBuffer();
      const worker = new Worker(WORKER_URL, { type: "module" });
      console.log(worker);
      worker.onerror = (e) => {
        reject(e);
      };
      worker.postMessage({
        wasmData: wasmDataBuffer
      }, [wasmDataBuffer]);
      worker.onmessage = (msg) => {
        const gama = new _Gama(worker);
        if (msg.data.ok)
          resolve(gama);
        else
          reject(msg.data.error);
      };
    });
  }
  async start() {
    this.worker.onmessage = (e) => this.handleWorkerMessage(e);
    this.worker.postMessage({
      buffer: this.buffer
    });
  }
  async handleWorkerMessage(msg) {
    switch (msg.data.type) {
      case "resize":
        var [w, h] = msg.data.size;
        if (w == 0 && h == 0) {
          this.sizemode = "natural";
          this.updateSize();
        } else {
          this.sizemode = "fixed";
          this.resize(w, h);
        }
        break;
      case "set-title":
        this.setTitle(msg.data.title);
        break;
      case "set-background-color":
        this.setBackground(msg.data.color);
        break;
      case "fullscreen":
        this.setFullscreen(msg.data.fullscreen);
        break;
      case "draw":
        for (const cmd of msg.data.cmds) {
          this.drawCmd(cmd);
        }
        break;
      case "yield":
        const gen = this.yield();
        await gen.next();
        Atomics.store(this.buffer32, 0, 1);
        Atomics.notify(this.buffer32, 0);
        writeYieldResult(this.buffer, 1, this.yielding);
        await gen.next();
        break;
    }
  }
  workerError(e) {
    console.error("Error running gama web worker: ", e);
  }
  async *yield() {
    this.ctx.back.clearRect(0, 0, this.canvas.back.width, this.canvas.back.height);
    this.ctx.back.drawImage(this.canvas.front, 0, 0);
    this.ctx.front.clearRect(0, 0, this.canvas.front.width, this.canvas.front.height);
    yield;
    await new Promise((resolve) => {
      requestAnimationFrame(() => {
        this.output?.clearRect(0, 0, this.output.canvas.width, this.output.canvas.height);
        this.output?.drawImage(this.canvas.back, 0, 0);
        resolve();
      });
    });
    yield;
  }
  drawCmd(_cmd) {
    const [cmd, ...args] = _cmd;
    const ctx = this.ctx.front;
    switch (cmd) {
      case "line":
        var [x1, y1, x2, y2, s, c] = args;
        ctx.beginPath();
        this._stroke(c);
        ctx.moveTo(...this._c_coord(x1, y1));
        ctx.lineTo(...this._c_coord(x2, y2));
        ctx.closePath();
        ctx.stroke();
        break;
      case "rect":
        var [x, y, w, h, c] = args;
        this._fill(c);
        var [x, y] = this._c_coord(x, y);
        var [w, h] = [this._c_one(w), this._c_one(h)];
        ctx.fillRect(x - w / 2, y - h / 2, w, h);
        break;
      case "roundrect":
        var [x, y, w, h, r, c] = args;
        var [w, h] = [this._c_one(w), this._c_one(h)];
        var [topX, topY] = this._c_coord(x, y);
        topX -= w / 2;
        topY -= h / 2;
        var r = this._c_one(r);
        if (w < 2 * r) r = w / 2;
        if (h < 2 * r) r = h / 2;
        this._fill(c);
        ctx.beginPath();
        ctx.moveTo(topX + r, topY);
        ctx.arcTo(topX + w, topY, topX + w, topY + h, r);
        ctx.arcTo(topX + w, topY + h, topX, topY + h, r);
        ctx.arcTo(topX, topY + h, topX, topY, r);
        ctx.arcTo(topX, topY, topX + w, topY, r);
        ctx.closePath();
        ctx.fill();
        break;
      case "triangle":
        var [x1, y1, x2, y2, x3, y3, col] = args;
        this._fill(col);
        this._stroke(col);
        ctx.beginPath();
        ctx.moveTo(...this._c_coord(x1, y1));
        ctx.lineTo(...this._c_coord(x2, y2));
        ctx.lineTo(...this._c_coord(x3, y3));
        ctx.closePath();
        ctx.fill();
        ctx.stroke();
        break;
      case "circle":
        var [x, y, r, c] = args;
        ctx.beginPath();
        ctx.arc(...this._c_coord(x, y), this._c_one(r), 0, 2 * Math.PI);
        this._fill(c);
        ctx.fill();
        break;
      case "text":
        var [x, y, s, txt, font, style, c] = args;
        ctx.font = this._c_one(s).toFixed(0) + "px '" + font + "'";
        this._fill(c);
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.fillText(txt, ...this._c_coord(x, y));
        break;
    }
  }
  _stroke(col) {
    this.ctx.front.strokeStyle = gmcToCss(col);
  }
  _fill(col) {
    this.ctx.front.fillStyle = gmcToCss(col);
  }
  _c_coord(x, y) {
    let norm_x = (x + 1) * 0.5;
    let norm_y = (1 - y) * 0.5;
    return [norm_x * this.window.side + this.window.offset.x, norm_y * this.window.side + this.window.offset.y];
  }
  _js_offset(x, y) {
    return [x + this.window.offset.x, y + this.window.offset.y];
  }
  _js_unoffset(x, y) {
    return [x - this.window.offset.x, y - this.window.offset.y];
  }
  _js_coord(x, y) {
    let norm_x = (x - this.window.offset.x) / this.window.side;
    let norm_y = (y - this.window.offset.y) / this.window.side;
    return [norm_x * 2 - 1, 1 - norm_y * 2];
  }
  _c_one(v) {
    return v * this.window.side * 0.5;
  }
  _js_one(v) {
    return v * 2 / this.window.side;
  }
  _c_rect(x, y, w, h) {
    let [gx, gy] = this._c_coord(x, y);
    let gw = w * 0.5 * this.window.side;
    let gh = h * 0.5 * this.window.side;
    return [gx - gw / 2, gy - gh / 2, gw, gh];
  }
  setFullscreen(fs) {
    if (fs) {
      if (this.output)
        this.output.canvas.requestFullscreen();
    } else {
      if (document.fullscreenElement)
        document.exitFullscreen();
    }
  }
  setBackground(col) {
    if (this.output) {
      this.output.canvas.style.backgroundColor = gmcToCss(col);
      this.output.canvas.style.background = gmcToCss(col);
      console.log(col, gmcToCss(col));
    } else
      console.error("Gama instance has no output");
  }
  setTitle(msg) {
    try {
      document.querySelector("title").innerHTML = msg;
    } catch (e) {
      console.error(e);
    }
  }
  resize(width, height) {
    this.canvas.front.width = width;
    this.canvas.front.height = height;
    this.canvas.back.width = width;
    this.canvas.back.height = height;
    if (this.output) {
      this.output.canvas.width = width;
      this.output.canvas.height = height;
    }
    this.window.side = Math.min(width, height);
    this.window.offset.x = (width - this.window.side) / 2;
    this.window.offset.y = (height - this.window.side) / 2;
  }
  attach(canv) {
    this.output = canv.getContext("2d");
    const cb = () => {
      if (this.sizemode == "natural") this.updateSize();
    };
    try {
      window.addEventListener("resize", cb);
    } catch (e) {
    }
    canv.addEventListener("resize", cb);
    this.bindKeyboard(canv);
    this.bindMouse(canv);
  }
  updateSize() {
    if (this.output) {
      const rect = this.output.canvas.getBoundingClientRect();
      this.resize(rect.width, rect.height);
    }
  }
  bindKeyboard(elt) {
    elt.addEventListener("keydown", (e) => {
      this.yielding.keyboard.down.push(getKeyCode(e.key));
    });
    elt.addEventListener("keyup", (e) => {
      const code = getKeyCode(e.key);
      this.yielding.keyboard.down = this.yielding.keyboard.down.filter((c) => c != code);
    });
  }
  bindMouse(elt) {
    elt.addEventListener("mousemove", (e) => {
      const r = elt.getBoundingClientRect();
      const coords = this._js_coord(e.clientX - r.x, e.clientY - r.y);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
    });
    elt.addEventListener("mousedown", (e) => {
      const r = elt.getBoundingClientRect();
      const coords = this._js_coord(e.clientX - r.x, e.clientY - r.y);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
      this.yielding.mouse.down = true;
    });
    elt.addEventListener("mouseup", (e) => {
      const r = elt.getBoundingClientRect();
      const coords = this._js_coord(e.clientX - r.x, e.clientY - r.y);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
      this.yielding.mouse.down = false;
    });
    const touchpos = (e) => {
      const r = elt.getBoundingClientRect();
      return this._js_coord(
        e.touches[0].clientX - r.x,
        e.touches[0].clientY - r.y
      );
    };
    elt.addEventListener("touchmove", (e) => {
      const coords = touchpos(e);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
    });
    elt.addEventListener("touchstart", (e) => {
      const coords = touchpos(e);
      this.yielding.mouse.x = coords[0];
      this.yielding.mouse.y = coords[1];
      this.yielding.mouse.down = true;
    });
    const handle = () => {
      this.yielding.mouse.down = false;
    };
    elt.addEventListener("touchcancel", handle);
    elt.addEventListener("touchend", handle);
  }
};
export {
  Gama as default
};
