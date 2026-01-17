export type GmKeyCode = string;
export declare const keyCode: (t: number, k: number) => string;
export declare function getKeyCode(key: string): GmKeyCode;
export declare function getCodeKey(code: string): string | null;
export declare const KEYS: {
    [key: string]: string;
};
