import Bluray from '../dist/binding.js';
import assert from 'assert';

const BD_DISK_PATH = './test/Sintel-Bluray.iso';

const { major, minor, micro } = Bluray.version();
console.log(`Using libbluray v${major}.${minor}.${micro}`);

const initTests = [
    [
        function InitOpenClose() {
            console.log('[INIT] Initialize, open, close');
            const bluray = new Bluray();
            assert(!bluray.isOpen(), '[INIT] Empty bluray not initialized correctly');
            assert(bluray.open(BD_DISK_PATH), '[INIT] Bluray open method not successful');
            assert(bluray.close(), 'Unable to close bluray disc');
        }, false
    ], [
        function InitWithPath() {
            console.log('[INIT] Initialize with path, close');
            const bluray = new Bluray(BD_DISK_PATH);
            assert(bluray.isOpen(), '[INIT] Bluray not initialized correctly');
            assert(bluray.close(), 'Unable to close bluray disc');
        }, false
    ], [
        function CloseWithoutOpen() {
            console.log('[INIT] Init, close (throws)');
            const bluray = new Bluray();
            bluray.close();
        }, true
    ]
];

initTests.forEach(([test, throws], n) => {
    const failureText = `[INIT] Test ${n + 1} failed.`;
    if (throws) assert.throws(test, undefined, failureText);
    else assert.doesNotThrow(test, undefined, failureText);
});

console.log('All tests successful!');