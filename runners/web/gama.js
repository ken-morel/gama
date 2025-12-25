import getKey from "./keys.js";

export class GamaInstance {
  constructor() {
    this.contexts = [];
    this.offset = { x: 0, y: 0 };
  }
  bind(canvas) {
    this.contexts.push(canvas.getContext('2d'));
    canvas.addEventListener('mousemove', e => {
      this.worker.postMessage({
        type: 'event/mousemove',
        movement: [this._js_one(e.movementX), this._js_one(e.movementY)],
        position: [...this._js_coord(e.x, e.y)]
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
    canvas.addEventListener('keydown', e => {
      this.worker.postMessage({
        type: 'event/keydown',
        key: getKey(e.key),
      });
    });
    canvas.addEventListener('keyup', e => {
      this.worker.postMessage({
        type: 'event/keyup',
        key: getKey(e.key),
      });
    });
  }
  async setup(workerPath, wasmPath) {
    this.worker = new Worker(workerPath, { type: 'module' });
    this.worker.onmessage = (msg) => this.handleWorkerMessage(this, msg);
    this.worker.onerror = this.handleWorkerError;
    this.worker.postMessage(wasmPath);
  }
  destroy() {
    console.info("stopping worker");
    this.worker.terminate();
  }
  yield() {
    requestAnimationFrame(() => {
      for (const context of this.contexts) {
        context.clearRect(0, 0, context.canvas.width, context.canvas.height);
        context.drawImage(this.canv, 0, 0);
      }
      this.ctx.clearRect(0, 0, this.ctx.canvas.width, this.ctx.canvas.height);
      this.worker.postMessage(null);
    });
  }
  handleWorkerMessage(gama, event) {
    const ctx = gama.ctx;
    const d = event.data;
    if (d == null) { // loop completed, save and wait for next frame
      this.yield();
      return;
    }
    switch (d.type) {
      case 'initialize':
        console.log("received initialize message from gama", d);
        gama.canv = new OffscreenCanvas(d.width, d.height);
        gama.canv.width = d.width;
        gama.canv.height = d.height;
        gama.size = { x: d.width, y: d.height };
        gama.applySize();
        gama.ctx = gama.canv.getContext('2d');
        try {
          document.querySelector('title').innerHTML = d.title;
        } catch (e) { }
        this.yield();
        break;
      case 'draw/line':
        var { start, stop, color, size } = d;
        ctx.beginPath();
        ctx.moveTo(...gama._c_coord(...start));
        ctx.lineTo(...gama._c_coord(...stop));
        gama._stroke(...color);
        ctx.lineWidth = gama._c_one(size);
        ctx.stroke();
        break;
      case 'set/bg':
        var col = gama._col(...d.color);
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
        var [w, h] = size;
        var [x, y] = pos;
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

      case 'draw/triangle':
        var { a, b, c, color } = d;

        gama._fill(...color);

        ctx.beginPath();
        ctx.moveTo(...this._c_coord(...a));
        ctx.lineTo(...this._c_coord(...b));
        ctx.lineTo(...this._c_coord(...c));
        ctx.closePath();
        ctx.fill();

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
        ctx.canvas.width = width;
        ctx.canvas.height = width;
        for (var context in this.contexts) {
          context.canvas.width = width;
          context.canvas.height = width;
        }
        break;
      case 'fullscreen':
        //TODO: find the currently viewed bound canvas
        try {
          if (d.full) {
            this.contexts[0].canvas.requestFullscreen();
          } else {
            document.exitFullscreen();
          }

        } catch (e) { }
        break;

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

    return [norm_x * this.size.x + this.offset.x, norm_y * this.size.y +
      this.offset.y];
  }

  _js_coord(x, y) {
    let norm_x = (x - this.offset.x) / this.size.x
    let norm_y = (y - this.offset.y) / this.size.y

    return [(norm_x * 2) - 1.0, 1.0 - (norm_y * 2)]
  }

  _c_one(v) {
    return v * this.size.x * 0.5
  }

  _js_one(v) {
    return (v * 2) / this.ctx.canvas.width;
  }

  _c_rect(x, y, w, h) {
    let gx, gy = this._c_coord(x, y)

    let gw = w * 0.5 * this.size.x
    let gh = h * 0.5 * this.size.y

    return [gx - gw / 2, gy - gh / 2, gw, gh];
  }
}
