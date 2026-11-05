#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "modCurrentSource.h"

int main(void) {
    uint8_t frame[8] = {0x80,0,0,0,0,0,0,0};
    int32_t ma = 999;
    assert(modCurrentSourceDecodeLEM(frame, &ma) && ma == 0);
    frame[2] = 0x27; frame[3] = 0x10;
    assert(modCurrentSourceDecodeLEM(frame, &ma) && ma == 10000);
    frame[0] = 0x7f; frame[1] = 0xff; frame[2] = 0xd8; frame[3] = 0xf0;
    assert(modCurrentSourceDecodeLEM(frame, &ma) && ma == -10000);
    frame[4] = 1;
    assert(!modCurrentSourceDecodeLEM(frame, &ma));
    modCurrentSourceState s = {0};
    modCurrentSourceUpdate(&s, false, false, 0);
    assert(s.ready && !s.fault); /* shunt is unaffected by LEM loss */
    modCurrentSourceUpdate(&s, true, false, 1);
    assert(!s.ready && !s.fault);
    modCurrentSourceUpdate(&s, true, true, 20); /* zero amps is still fresh */
    modCurrentSourceUpdate(&s, true, true, 1019);
    assert(!s.ready);
    modCurrentSourceUpdate(&s, true, true, 1020);
    assert(s.ready && !s.fault);
    modCurrentSourceUpdate(&s, true, false, 1220);
    assert(!s.ready && s.fault);
    modCurrentSourceUpdate(&s, true, true, 5000);
    assert(!s.ready && s.fault); /* no auto rearm */
    modCurrentSourceUpdate(&s, false, true, 5001);
    assert(!s.ready && s.fault); /* no shunt fallback via config write */

    s = (modCurrentSourceState){0};
    modCurrentSourceUpdate(&s, true, false, UINT32_MAX - 100);
    modCurrentSourceUpdate(&s, true, false, 2898);
    assert(!s.fault);
    modCurrentSourceUpdate(&s, true, false, 2899);
    assert(s.fault); /* wraparound-safe startup deadline */

    s = (modCurrentSourceState){0};
    modCurrentSourceUpdate(&s, true, true, 0);
    modCurrentSourceUpdate(&s, true, false, 500);
    modCurrentSourceUpdate(&s, true, true, 600);
    modCurrentSourceUpdate(&s, true, true, 1599);
    assert(!s.ready);
    modCurrentSourceUpdate(&s, true, true, 1600);
    assert(s.ready); /* a startup gap restarts qualification */
    puts("current-source policy: all checks passed");
}
