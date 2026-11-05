#ifndef MOD_CURRENT_SOURCE_H
#define MOD_CURRENT_SOURCE_H

#include <stdbool.h>
#include <stdint.h>

/* CAB500 offset-binary, big endian milliamps; byte 4 bit 0 is sensor error. */
static inline bool modCurrentSourceDecodeLEM(const uint8_t data[8], int32_t *milliamps) {
    if ((data[4] & 1U) != 0U) return false;
    uint32_t raw = ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) |
                   ((uint32_t)data[2] << 8) | data[3];
    *milliamps = (int32_t)((int64_t)raw - 2147483648LL);
    return true;
}

#define MOD_CURRENT_SOURCE_QUALIFY_MS 1000U
#define MOD_CURRENT_SOURCE_STARTUP_MS 3000U

typedef struct {
    bool initialized;
    bool lem;
    bool qualifying;
    bool ready;
    bool fault;
    uint32_t started;
    uint32_t healthySince;
} modCurrentSourceState;

/* fault is deliberately never cleared here: only a BMS restart rearms it. */
static inline void modCurrentSourceUpdate(modCurrentSourceState *state,
                                         bool lem, bool fresh, uint32_t now) {
    if (!state->initialized || state->lem != lem) {
        state->initialized = true;
        state->lem = lem;
        state->qualifying = false;
        state->ready = false;
        state->started = now;
    }
    if (state->fault) {
        state->ready = false;
        return;
    }
    if (!lem) {
        state->ready = true;
        return;
    }
    if (!fresh) {
        if (state->ready) state->fault = true;
        state->ready = false;
        state->qualifying = false;
    } else if (!state->ready) {
        if (!state->qualifying) {
            state->qualifying = true;
            state->healthySince = now;
        }
        if ((uint32_t)(now - state->healthySince) >= MOD_CURRENT_SOURCE_QUALIFY_MS)
            state->ready = true;
    }
    if (!state->ready && (uint32_t)(now - state->started) >= MOD_CURRENT_SOURCE_STARTUP_MS)
        state->fault = true;
}
#endif
