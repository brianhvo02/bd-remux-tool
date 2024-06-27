import { createRequire } from 'node:module';

export interface LibVersion {
    major: number;
    minor: number;
    micro: number;
}

export interface DiscInfo {
    blurayDetected: boolean;
    discName: string;
    udfVolumeId: string;
    discId: ArrayBuffer;
    noMenuSupport: boolean;
    firstPlaySupported: boolean;
    topMenuSupported: boolean;
    numTitles: number;
    titles: TitleInfo[];
    firstPlay: TitleInfo;
    topMenu: TitleInfo;
    numHdmvTitles: number;
    numBdjTitles: number;
    numUnsupportedTitles: number;
    bdjDetected: boolean;
    bdjSupported: boolean;
    libjvmDetected: boolean;
    bdjHandled: boolean;
    bdjOrgId: string;
    bdjDiscId: string;
    videoFormat: number;
    frameRate: number;
    contentExist3D: boolean;
    initialOutputModePreference: number;
    providerData: ArrayBuffer;
    aacsDetected: boolean;
    libaacsDetected: boolean;
    aacsHandled: boolean;
    aacsErrorCode: number;
    aacsMkbv: number;
    bdplusDetected: boolean;
    libbdplusDetected: boolean;
    bdplusHandled: boolean;
    bdplusGen: number;
    bdplusDate: number;
    initialDynamicRangeType: number;
}

export interface StreamInfo {
    codingType: number;
    format: number;
    rate: number;
    charCode: number;
    lang: number[];
    pid: number;
    aspect: number;
    subpathId: number;
}

export interface ClipInfo {
    pktCount: number;
    stillMode: number;
    stillTime: number;
    videoStreamCount: number;
    audioStreamCount: number;
    pgStreamCount: number;
    igStreamCount: number;
    secAudioStreamCount: number;
    secVideoStreamCount: number;
    videoStreams: StreamInfo[];
    audioStreams: StreamInfo[];
    pgStreams: StreamInfo[];
    igStreams: StreamInfo[];
    secAudioStreams: StreamInfo[];
    secVideoStreams: StreamInfo[];
    startTime: number;
    inTime: number;
    outTime: number;
    clipId: string;
}

export interface TitleChapter {
    idx: number;
    start: number;
    duration: number;
    offset: number;
    clipRef: number;
}

export interface TitleMark extends TitleChapter {
    type: number;
}

export interface TitleInfo {
    idx: number;
    playlist: number;
    duration: number;
    clipCount: number;
    angleCount: number;
    chapterCount: number;
    clips: ClipInfo[];
    chapters: TitleChapter[];
    marks: TitleMark[];
    markCount: number;
    mvcBaseViewRFlag: number;
}

export interface AddonBluray {
    new(path?: string): AddonBluray;
    version(): LibVersion;
    open(path?: string): string;
    isOpen(): boolean;
    close(): boolean;
    getDiscInfo(): DiscInfo;
    getTitles(flags?: number, minLength?: number): number;
    getMainTitle(): number;
    getTitleInfo(title: number, angle?: number): TitleInfo;
    getPlaylistInfo(playlistId: number, angle?: number): TitleInfo;
}

const addon: {
    Bluray: AddonBluray;
} = createRequire(import.meta.url)("../build/Release/bluray-bindings-native.node");

export default class Bluray {
    constructor(path?: string) {
        this._addonInstance = path
            ? new addon.Bluray(path)
            : new addon.Bluray()
    }
    
    static version() {
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

    getDiscInfo() {
        return this._addonInstance.getDiscInfo();
    }

    getTitles(flags?: number, minLength?: number) {
        return this._addonInstance.getTitles(flags, minLength);
    }
    
    getMainTitle() {
        return this._addonInstance.getMainTitle();
    }

    getTitleInfo(title: number, angle?: number) {
        return this._addonInstance.getTitleInfo(title, angle);
    }

    getPlaylistInfo(playlistId: number, angle?: number) {
        return this._addonInstance.getPlaylistInfo(playlistId, angle);
    }

    // private members
    private _addonInstance: AddonBluray;
}
