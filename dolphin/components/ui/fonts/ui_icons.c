#include "ui_icons.h"

// =======================
// Battery icons (8x8)
// =======================

// Empty battery
const uint8_t icon_batt_empty[8] = {
    0x18, // ...██...  cap
    0x7E, // .██████. top border
    0x42, // .█....█.
    0x42, // .█....█.
    0x42, // .█....█.
    0x42, // .█....█.
    0x42, // .█....█.
    0x7E  // .██████. bottom border
};

// Half battery (2 of 3 levels filled)
const uint8_t icon_batt_half[8] = {
    0x18, // ...██...  cap
    0x7E, // .██████.
    0x42, // .█....█.
    0x42, // .█....█. empty top
    0x7E, // .██████. mid
    0x7E, // .██████. mid
    0x7E, // .██████. bottom
    0x7E  // .██████. border
};

// Full battery (all 3 levels filled)
const uint8_t icon_batt_full[8] = {
    0x18, // ...██...  cap
    0x7E, // .██████.
    0x7E, // .██████.
    0x7E, // .██████.
    0x7E, // .██████.
    0x7E, // .██████.
    0x7E, // .██████.
    0x7E  // .██████.
};

// Low signal: 1 short bar at bottom
const uint8_t icon_wifi_1[8] = {
    0x00, // ........
    0x00, // ........
    0x00, // ........
    0x00, // ........
    0x00, // ........
    0x00, // ........
    0x3C, // ..████..
    0x3C  // ..████..
};

// Medium signal: 2 bars (gap in between)
const uint8_t icon_wifi_2[8] = {
    0x00, // ........
    0x00, // ........
    0x00, // ........
    0x3C, // ..████.. (upper bar)
    0x3C, // ..████..
    0x00, // ........ (gap)
    0x3C, // ..████.. (lower bar)
    0x3C  // ..████..
};

// Good signal: 3 bars (with gaps)
const uint8_t icon_wifi_3[8] = {
    0x3C, // ..████.. (top bar)
    0x3C, // ..████..
    0x00, // ........ (gap)
    0x3C, // ..████.. (middle bar)
    0x3C, // ..████..
    0x00, // ........ (gap)
    0x3C, // ..████.. (bottom bar)
    0x3C  // ..████..
};


