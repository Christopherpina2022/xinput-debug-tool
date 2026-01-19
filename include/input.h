#pragma once
#include <stdint.h>

#define MAX_CONTROLLERS 4

typedef enum {
    BTN_A       = 1 << 0,
    BTN_B       = 1 << 1,
    BTN_X       = 1 << 2,
    BTN_Y       = 1 << 3,

    BTN_LB      = 1 << 4,
    BTN_RB      = 1 << 5,

    BTN_BACK    = 1 << 6,
    BTN_START   = 1 << 7,

    BTN_LS      = 1 << 8,
    BTN_RS      = 1 << 9,

    BTN_DPAD_UP    = 1 << 10,
    BTN_DPAD_DOWN  = 1 << 11,
    BTN_DPAD_LEFT  = 1 << 12,
    BTN_DPAD_RIGHT = 1 << 13
} GamepadButtons;

typedef struct {
    int connected;
    int16_t lx, ly;
    int16_t rx, ry;
    uint8_t lt, rt;
    uint16_t buttons;
} GamepadState;

void input_init(void);
void input_update(void);
const GamepadState *input_get_gamepad(int index);