import { GmKeyCode } from "./keyboard";
export type YieldResult = {
    mouse: {
        x: number;
        y: number;
        down: boolean;
    };
    keyboard: {
        down: Set<GmKeyCode>;
    };
};
export declare function writeYieldResult(buf: SharedArrayBuffer, offset: number, res: YieldResult): void;
export declare function readYieldResult(buf: SharedArrayBuffer, offset: number): YieldResult;
