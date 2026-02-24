/**
 * Represents a normalized Gama key code, used for consistent input handling
 * between JavaScript and the WebAssembly game. Each code is a two-character string.
 */
export type GmKeyCode = string;
/**
 * Creates a GmKeyCode from two number components.
 * This is primarily for internal use to generate unique key codes.
 * @param t The type component of the key code.
 * @param k The key component of the key code.
 * @returns A two-character GmKeyCode string.
 */
export declare const keyCode: (t: number, k: number) => string;
/**
 * Converts a standard keyboard `key` string (e.g., "ArrowUp", "a", "Enter")
 * to a normalized Gama key code (`GmKeyCode`).
 * If the key is not recognized, it returns a default "  " code.
 * @param key The standard keyboard key string.
 * @returns The corresponding GmKeyCode.
 */
export declare function getKeyCode(key: string): GmKeyCode;
/**
 * Converts a normalized Gama key code (`GmKeyCode`) back to a standard keyboard `key` string.
 * @param code The GmKeyCode to convert.
 * @returns The corresponding standard keyboard key string, or `null` if not found.
 */
export declare function getCodeKey(code: string): string | null;
/**
 * A mapping from standard keyboard `key` strings to normalized Gama key codes.
 * This table defines the internal representation of keys for efficient handling.
 *
 * Key classes:
 * - `a`: Arrow keys
 * - `s`: Special keys (Escape, Insert, Delete, etc.)
 * - `m`: Modifiers (Shift, Control, Alt, Meta)
 * - `c`: Characters (alphanumeric, punctuation)
 * - `f`: Function keys (F1-F12)
 *
 * Example: "ArrowUp" maps to "au", "a" maps to "ca", "F1" maps to "f1".
 */
export declare const KEYS: {
    [key: string]: string;
};
