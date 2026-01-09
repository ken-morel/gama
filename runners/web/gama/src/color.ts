export type GmColor = number;

export function gmcDecode(col: GmColor): { r: number, g: number, b: number, a: number } {
  col = col >>> 0;
  return {
    r: (col >>> 24) & 0xFF,
    g: (col >>> 16) & 0xFF,
    b: (col >>> 8) & 0xFF,
    a: col & 0xFF,
  };
}

export function gmcEncode(r: number, g: number, b: number, a: number): GmColor {
  return (r << 24) | (g << 16) | (b << 8) | a;
}

export function gmcToCss(c: GmColor): string {
  const { r, g, b, a } = gmcDecode(c);
  const normalizedAlpha = (a / 255).toFixed(2);
  return `rgba(${r}, ${g}, ${b}, ${normalizedAlpha})`;
}
