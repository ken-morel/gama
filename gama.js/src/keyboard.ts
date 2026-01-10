export type GmKeyCode = string

export const keyCode = (t: number, k: number) => String.fromCodePoint(t, k);


export function getKeyCode(key: string): GmKeyCode {
  return (KEYS[key] || '  ') as GmKeyCode;
}
export function getCodeKey(code: string): string | null {
  if (code === '  ') return null;
  for (let k in KEYS) {
    if (KEYS[k] == code)
      return k;
  }
  return null;
}

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
