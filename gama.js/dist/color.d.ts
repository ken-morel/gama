/**
 * Represents a Gama color in a single 32-bit number (RGBA format: RRRRGGGGAAAABBBB).
 */
export type GmColor = number;
/**
 * Decodes a Gama color number into its RGBA components.
 * @param col The Gama color number.
 * @returns An object with r, g, b, and a components (0-255).
 */
export declare function gmcDecode(col: GmColor): {
    r: number;
    g: number;
    b: number;
    a: number;
};
/**
 * Encodes RGBA components into a single Gama color number.
 * @param r The red component (0-255).
 * @param g The green component (0-255).
 * @param b The blue component (0-255).
 * @param a The alpha component (0-255).
 * @returns The encoded Gama color number.
 */
export declare function gmcEncode(r: number, g: number, b: number, a: number): GmColor;
/**
 * Converts a Gama color number to a CSS `rgba()` string.
 * @param c The Gama color number.
 * @returns A CSS `rgba()` string (e.g., "rgba(255, 0, 0, 1.00)").
 */
export declare function gmcToCss(c: GmColor): string;
