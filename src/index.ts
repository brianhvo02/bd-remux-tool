import { mkdtemp, rename, rm, writeFile } from 'fs/promises';
import Bluray, { StreamInfo } from '../libbluray-bindings/dist/binding.js';
import { join } from 'path';
import openEditor from 'open-editor';
import { spawn } from 'child_process';
import { tmpdir } from 'os';

const BD_DISK_PATH = '/Users/brianhvo02/Desktop/Volume 1';
const bluray = new Bluray(BD_DISK_PATH);
// const info = bluray.getDiscInfo();
// console.log(info);

const playlistCount = bluray.getTitles();
const playlists = [...Array(playlistCount).keys()].map(i => bluray.getPlaylistInfo(i));

// const titleCount = info.numTitles + Number(info.firstPlaySupported) + Number(info.topMenuSupported);
// const titles = [...Array(titleCount).keys()].map(i => bluray.getTitleInfo(i));

const createTmpDir = () => mkdtemp(join(tmpdir(), 'bd_remux_tool-'));

const getStreamType = (codingType: number) => {
    switch (codingType) {
        case 0x02:
            return 'V_MPEG-2';
        case 0x03:
        case 0x04:
            return 'A_MP3';
        case 0x80:
            return 'A_LPCM';
        case 0x81:
        case 0x84:
        case 0x83:
            return 'A_AC3';
        case 0x82:
        case 0x85:
        case 0x86:
            return 'A_DTS';
        case 0xea:
            return 'V_MS/VFW/WVC1';
        case 0x1b:
            return 'V_MPEG4/ISO/AVC';
        case 0x20:
            return 'V_MPEG4/ISO/MVC';
        case 0x24:
            return 'V_MPEGH/ISO/HEVC';
        case 0x90:
            return 'S_HDMV/PGS';
        case 0x92:
            return 'S_TEXT/UTF8';
        default:
            return 'UNKNOWN';
    }
}

const getStreamTypeHuman = (codingType: number) => {
    switch (codingType) {
        case 0x01:
            return 'MPEG-1 Video';
        case 0x02:
            return 'MPEG-2 Video';
        case 0x03:
            return 'MPEG-1 Audio';
        case 0x04:
            return 'MPEG-2 Audio';
        case 0x80:
            return 'LPCM';
        case 0x81:
            return 'AC-3';
        case 0x82:
            return 'DTS';
        case 0x83:
            return 'TrueHD';
        case 0x84:
            return 'AC-3 Plus';
        case 0x85:
            return 'DTS-HD';
        case 0x86:
            return 'DTS-HD Master';
        case 0xa1:
            return 'AC-3 Plus for secondary audio';
        case 0xa2:
            return 'DTS-HD for secondary audio';
        case 0xea:
            return 'VC-1';
        case 0x1b:
            return 'H.264';
        case 0x20:
            return 'H.264 MVC dep.';
        case 0x24:
            return 'HEVC';
        case 0x90:
            return 'Presentation Graphics';
        case 0x91:
            return 'Interactive Graphics';
        case 0x92:
            return 'Text subtitle';
        default:
            return 'UNKNOWN';
    }
}

const getStreamExt = (codingType: number) => {
    switch (codingType) {
        case 0x02:
            return '.mpv';
        case 0x03:
        case 0x04:
            return '.mp3';
        case 0x80:
            return '.wav';
        case 0x81:
        case 0x84:
        case 0x83:
            return '.ac3';
        case 0x82:
        case 0x85:
        case 0x86:
            return '.dts';
        case 0xea:
            return '.wvc1';
        case 0x1b:
            return '.264';
        case 0x20:
            return '.mvc';
        case 0x24:
            return '.hevc';
        case 0x90:
        case 0x92:
            return '.sup';
        default:
            return 'UNKNOWN';
    }
}

const getStream = (type: string, stream: StreamInfo): Stream => ({ 
    type, pid: stream.pid,
    codec: getStreamType(stream.codingType),
    codecHuman: getStreamTypeHuman(stream.codingType),
    codecExt: getStreamExt(stream.codingType),
    lang: stream.lang.filter(char => char !== 0x00)
        .map(char => String.fromCharCode(char)).join(''),
});

interface Chapter { 
    clipRef: number;
    start: number;
}

interface Stream { 
    type: string;
    pid: number;
    codec: string;
    codecHuman: string;
    codecExt: string;
    lang: string;
}

interface Title {
    filename: string;
    chapters: Chapter[];
    streams: Stream[];
}

const playlistTitles: Title[] = [];
const soloStreamTitles: Title[] = [];
const sameStreamTitles: Title[] = [];

playlists.forEach(playlist => {
    const { 
        videoStreams, secVideoStreams, 
        audioStreams, secAudioStreams, 
        pgStreams, igStreams
    } = playlist.clips[0];

    const streams = [
        ...videoStreams.map(stream => getStream('Video', stream)),
        // ...secVideoStreams.map(getStream.bind(null, 'Video')),
        ...audioStreams.map(stream => getStream('Audio', stream)),
        // ...secAudioStreams.map(getStream.bind(null, 'Audio')),
        ...pgStreams.map(stream => getStream('Subtitles', stream)),
        // ...igStreams.map(getStream.bind(null, 'Subtitles'))
    ];

    if (playlist.clipCount === 1)
        return soloStreamTitles.push({
            filename: playlist.clips[0].clipId + '.m2ts',
            chapters: playlist.chapters,
            streams,
        });
    
    const clipSet = new Set();
    playlist.clips.forEach(clip => clipSet.add(clip.clipId));
    if (clipSet.size === 1)
        return sameStreamTitles.push({
            filename: playlist.clips[0].clipId + '.m2ts',
            chapters: playlist.chapters,
            streams,
        });
    
    playlistTitles.push({
        filename: playlist.playlist.toString().padStart(5, '0') + '.mpls',
        chapters: playlist.chapters,
        streams,
    });
});

const titles = playlistTitles.concat(soloStreamTitles).concat(sameStreamTitles);
titles.forEach((title, i) => {
    console.log(`File ${title.filename} was added as title #${i}`);
});
console.log(`Total ${titles.length} titles`);

// titles.forEach((title, i) => {
//     console.log('Title ' + i);
//     title.streams.forEach(({ type, codecHuman }, i) => console.log(`${i} ${type} ${codecHuman}`));
//     console.log();
// });

const tmpDir = await createTmpDir();
const metaPath = join(tmpDir, 'demux.meta');
const chapterPath = join(tmpDir, 'chapters.txt');

const title = titles[2];

const metaLines = title.streams.map(({ codec, pid, lang }) => `${codec}, "${
    join(BD_DISK_PATH, 'BDMV', title.filename.includes('.mpls') ? 'PLAYLIST' : 'STREAM',
        title.filename)}", track=${pid}${lang && (', lang=' + lang)}`);

await writeFile(metaPath, ['MUXOPT --demux', ...metaLines].join('\n'));

const convertTimestamp = (time: number) => {
    time /= 90;
    const ms = time % 1000;
    time = (time - ms) / 1000;
    const secs = time % 60;
    time = (time - secs) / 60;
    const mins = time % 60;
    const hrs = (time - mins) / 60;
    return `${
        hrs.toString().padStart(2, '0')}:${
        mins.toString().padStart(2, '0')}:${
        secs.toString().padStart(2, '0')}.${
        Math.round(ms).toString().padStart(3, '0')
    }`;
}

const clipSplit: number[] = [];
// title.chapters.reduce((arr, { clipRef }, i) => arr[clipRef] > -1 ? arr : arr.concat(i + 1), []);
const chapterLines = title.chapters.map(({ clipRef, start }, i) => {
    if (clipSplit[clipRef] === undefined)
        clipSplit[clipRef] = i + 1;
    const totalNum = (i + 1).toString().padStart(2, '0');
    const num = (i + 2 - clipSplit[clipSplit.length - 1]).toString().padStart(2, '0');
    return `CHAPTER${totalNum}=${convertTimestamp(start)}\nCHAPTER${totalNum}NAME=Chapter ${num}`;
});

await writeFile(chapterPath, chapterLines.join('\n'));

await openEditor([chapterPath], { wait: true });

await new Promise<void>((resolve, reject) => {
    const proc = spawn('tsmuxer', [metaPath, tmpDir]);
    proc.stdout.pipe(process.stdout);
    proc.stderr.pipe(process.stderr);
    proc.on('close', code => code === 0 ? resolve() : reject(code));
});

await new Promise<void>((resolve, reject) => {
    const args = [
        '--chapters', chapterPath,
        '--split', `chapters:${clipSplit.join(',')}`,
        '-o', join(tmpDir, 'mux_%02d.mkv'),
    ];

    title.streams.forEach(({ pid, lang, codec, codecExt }) => {
        if (lang) args.push('--language', `0:${lang}`);
        if (codec.slice(0, 1) === 'S') args.push('--default-track-flag', '0:0');
        args.push(join(tmpDir, `${title.filename.slice(0, 5)}.track_${pid}${lang && ('_' + lang)}${codecExt}`));
    });

    const proc = spawn('mkvmerge', args);
    proc.stdout.pipe(process.stdout);
    proc.stderr.pipe(process.stderr);
    proc.on('close', code => code === 0 ? resolve() : reject(code));
});

for (let i = 0; i < clipSplit.length; i++) {
    const filename = `mux_${(i + 1).toString().padStart(2, '0')}.mkv`;
    await rename(join(tmpDir, filename), join('output', filename));
}

await rm(tmpDir, { recursive: true });