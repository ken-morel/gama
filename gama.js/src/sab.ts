/**
 * @fileoverview SharedArrayBuffer (SAB) utilities for inter-thread communication.
 * This file defines structures and functions for writing and reading yield results
 * (mouse/keyboard state) to/from a SharedArrayBuffer, facilitating fast and efficient
 * data exchange between the main thread and the Web Worker.
 */

import { GmKeyCode, keyCode } from "./keyboard";

/**
 * Represents the input state (mouse and keyboard) that is passed
 * between the main thread and the Web Worker via SharedArrayBuffer.
 */
export type YieldResult = {
  /** Mouse input state. */
  mouse: {
    x: number,
    y: number,
    down: boolean
  },
  /** Keyboard input state. */
  keyboard: {
    /** A set of currently pressed normalized key codes. */
    down: Set<GmKeyCode>
  }
};

/**
 * Writes the current YieldResult (input state) into a SharedArrayBuffer.
 * Data is written starting at the specified offset.
 *
 * Layout:
 * - Mouse X (float32): 4 bytes
 * - Mouse Y (float32): 4 bytes
 * - Mouse Down (uint8): 1 byte
 * - Keyboard Key Count (uint8): 1 byte
 * - Keyboard Key Codes (array of uint8 pairs): 2 bytes per key
 *
 * @param buf The SharedArrayBuffer to write to.
 * @param offset The byte offset within the buffer to start writing.
 * @param res The YieldResult object containing the input state.
 */
export function writeYieldResult(buf: SharedArrayBuffer, offset: number, res: YieldResult): void {
  const view = new DataView(buf);

  // Write mouse data (x, y, down)
  view.setFloat32(offset, res.mouse.x, true);
  offset += 4;
  view.setFloat32(offset, res.mouse.y, true);
  offset += 4;
  view.setUint8(offset, res.mouse.down ? 1 : 0);
  offset += 1;

  // Write keyboard data (key count followed by key codes)
  const keyCount = res.keyboard.down.size;
  view.setUint8(offset, keyCount);
  offset += 1;

  for (const key of res.keyboard.down) {
    view.setUint8(offset, key.charCodeAt(0));
    view.setUint8(offset + 1, key.charCodeAt(1));
    offset += 2;
  }
}

/**
 * Reads input state (YieldResult) from a SharedArrayBuffer.
 * Data is read starting from the specified offset.
 * @param buf The SharedArrayBuffer to read from.
 * @param offset The byte offset within the buffer to start reading.
 * @returns The reconstructed YieldResult object.
 */
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

  const downKeys = new Set<GmKeyCode>();
  while (offset < end)
    downKeys.add(keyCode(view.getUint8(offset++), view.getUint8(offset++)));

  return {
    mouse,
    keyboard: { down: downKeys }
  };
}