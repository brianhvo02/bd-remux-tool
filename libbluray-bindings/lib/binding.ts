const addon = require('../build/Release/bluray-bindings-native');

interface LibVersion {
    major: number;
    minor: number;
    micro: number;
};

interface IBlurayNative {
    open(path: string): string;
    isOpen(): boolean;
    close(): boolean;
};

class Bluray {
    constructor(path?: string) {
        this._addonInstance = path
            ? new addon.Bluray(path)
            : new addon.Bluray()
    }
    
    static version(): LibVersion {
        return addon.Bluray.version();
    }

    open(path: string) {
        return this._addonInstance.open(path);
    }

    isOpen() {
        return this._addonInstance.isOpen();
    }

    close() {
        return this._addonInstance.close();
    }

    // private members
    private _addonInstance: IBlurayNative;
}

export = Bluray;
