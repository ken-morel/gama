
import { GmKeyCode, keyCode } from "./keyboard";

export type YieldResult = {
  mouse: { x: number, y: number, down: boolean },
  keyboard: { down: GmKeyCode[] }
};

export function writeYieldResult(buf: SharedArrayBuffer, offset: number, res: YieldResult) {
  const view = new DataView(buf);

  // Write mouse data (x, y, down)
  view.setFloat32(offset, res.mouse.x, true);
  offset += 4;
  view.setFloat32(offset, res.mouse.y, true);
  offset += 4;
  view.setUint8(offset, res.mouse.down ? 1 : 0);
  offset += 1;

  // Write keyboard data (key count followed by key codes)
  const keyCount = res.keyboard.down.length;
  view.setUint8(offset, keyCount);
  offset += 1;

  for (const key of res.keyboard.down) {
    view.setUint8(offset, key.charCodeAt(0));
    view.setUint8(offset, key.charCodeAt(1));
    offset += 2;
  }
}

export function readYieldResult(buf: SharedArrayBuffer, offset: number): YieldResult {
  const view = new DataView(buf);

  // Read mouse data
  const mouse = {
    x: view.getFloat32(offset, true),
    y: view.getFloat32(offset + 4, true),
    down: view.getUint8(offset + 8) !== 0
  };
  offset += 9;

  // Read keyboard data
  const keyCount = view.getUint8(offset);
  offset += 1;

  const end = offset + keyCount * 2;

  const downKeys: GmKeyCode[] = [];
  while (offset < end)
    downKeys.push(keyCode(view.getUint8(offset++), view.getUint8(offset++)));

  return {
    mouse,
    keyboard: { down: downKeys }
  };
}

