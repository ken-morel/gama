/**
 * Represents a Gama color in a single 32-bit number (RGBA format: RRRRGGGGAAAABBBB).
 */
export type GmColor = number;

/**
 * Decodes a Gama color number into its RGBA components.
 * @param col The Gama color number.
 * @returns An object with r, g, b, and a components (0-255).
 */
export function gmcDecode(col: GmColor): { r: number, g: number, b: number, a: number } {
  col = col >>> 0;
  return {
    r: (col >>> 24) & 0xFF,
    g: (col >>> 16) & 0xFF,
    b: (col >>> 8) & 0xFF,
    a: col & 0xFF,
  };
}

/**
 * Encodes RGBA components into a single Gama color number.
 * @param r The red component (0-255).
 * @param g The green component (0-255).
 * @param b The blue component (0-255).
 * @param a The alpha component (0-255).
 * @returns The encoded Gama color number.
 */
export function gmcEncode(r: number, g: number, b: number, a: number): GmColor {
  return (r << 24) | (g << 16) | (b << 8) | a;
}

/**
 * Converts a Gama color number to a CSS `rgba()` string.
 * @param c The Gama color number.
 * @returns A CSS `rgba()` string (e.g., "rgba(255, 0, 0, 1.00)").
 */
export function gmcToCss(c: GmColor): string {
  const { r, g, b, a } = gmcDecode(c);
  const normalizedAlpha = (a / 255).toFixed(2);
  return `rgba(${r}, ${g}, ${b}, ${normalizedAlpha})`;
}