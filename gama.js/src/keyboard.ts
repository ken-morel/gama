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
export const keyCode = (t: number, k: number) => String.fromCodePoint(t, k);

/**
 * Converts a standard keyboard `key` string (e.g., "ArrowUp", "a", "Enter")
 * to a normalized Gama key code (`GmKeyCode`).
 * If the key is not recognized, it returns a default "  " code.
 * @param key The standard keyboard key string.
 * @returns The corresponding GmKeyCode.
 */
export function getKeyCode(key: string): GmKeyCode {
  return (KEYS[key] || '  ') as GmKeyCode;
}

/**
 * Converts a normalized Gama key code (`GmKeyCode`) back to a standard keyboard `key` string.
 * @param code The GmKeyCode to convert.
 * @returns The corresponding standard keyboard key string, or `null` if not found.
 */
export function getCodeKey(code: string): string | null {
  if (code === '  ') return null;
  for (let k in KEYS) {
    if (KEYS[k] == code)
      return k;
  }
  return null;
}

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
export const KEYS: {
  [key: string]: string,
} = {
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