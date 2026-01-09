export type GmColor = number;

export function gmcDecode(col: GmColor): { r: number, g: number, b: number, a: number } {
  col = col >>> 0;
  return {
    r: (col & 0xFF000000) >> 24,
    g: (col & 0x00FF0000) >> 16,
    b: (col & 0x0000FF00) >> 8,
    a: (col & 0x000000FF) >> 0,
  };
}

export function gmcEncode(r: number, g: number, b: number, a: number): GmColor {
  return (r << 24) | (g << 16) | (b << 8) | a;
}

export function gmcToCss(c: GmColor): string {
  const { r, g, b, a } = gmcDecode(c);
  return `rgba(${r}, ${g}, ${b}, ${a})`;
}
