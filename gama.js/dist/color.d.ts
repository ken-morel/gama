export type GmColor = number;
export declare function gmcDecode(col: GmColor): {
    r: number;
    g: number;
    b: number;
    a: number;
};
export declare function gmcEncode(r: number, g: number, b: number, a: number): GmColor;
export declare function gmcToCss(c: GmColor): string;
