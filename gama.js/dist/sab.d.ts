/**
 * @fileoverview SharedArrayBuffer (SAB) utilities for inter-thread communication.
 * This file defines structures and functions for writing and reading yield results
 * (mouse/keyboard state) to/from a SharedArrayBuffer, facilitating fast and efficient
 * data exchange between the main thread and the Web Worker.
 */
import { GmKeyCode } from "./keyboard";
/**
 * Represents the input state (mouse and keyboard) that is passed
 * between the main thread and the Web Worker via SharedArrayBuffer.
 */
export type YieldResult = {
    /** Mouse input state. */
    mouse: {
        x: number;
        y: number;
        down: boolean;
    };
    /** Keyboard input state. */
    keyboard: {
        /** A set of currently pressed normalized key codes. */
        down: Set<GmKeyCode>;
    };
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
export declare function writeYieldResult(buf: SharedArrayBuffer, offset: number, res: YieldResult): void;
/**
 * Reads input state (YieldResult) from a SharedArrayBuffer.
 * Data is read starting from the specified offset.
 * @param buf The SharedArrayBuffer to read from.
 * @param offset The byte offset within the buffer to start reading.
 * @returns The reconstructed YieldResult object.
 */
export declare function readYieldResult(buf: SharedArrayBuffer, offset: number): YieldResult;
