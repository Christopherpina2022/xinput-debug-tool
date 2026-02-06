#pragma once
#include <windows.h>
#include <input.h>
#include <hidsdi.h>
#include <hidpi.h>
#include <stdint.h>

#define MAX_USAGES 128
#define HID_MAP_UNUSED -1
#define DEADZONE 0.15f

typedef struct {
    int mappedEnum;   // INPUT_AXIS_LEFT_X, INPUT_AXIS_RT, etc
    int axisIndex;    // SDL / logical axis index (0,1,2,…)
    USHORT usage;     // RawInput usage for this axis (from DB)
    int capIndex;     // Assigned after devReg
} AxisMapping;

typedef struct {
    int mappedEnum;   // INPUT_BTN_A, INPUT_BTN_B, etc
    int buttonIndex;  // DB index (b0, b1, …) — semantic only
    USHORT usage;     // HID usage (Button Page, resolved in devReg)
} ButtonMapping;

typedef struct {
    InputDpad mappedEnum; // INPUT_DPAD_UP, etc
    uint32_t  bit;        // BTN_DPAD_UP, etc
} DpadMapping;


// Creates a device record that we call once per device. caps are short for capabilities.
typedef struct {
    // Buttons
    ButtonMapping buttons[MAX_USAGES];
    int buttonCount;

    // Axes
    AxisMapping axes[MAX_USAGES];
    int axisCapIndex[MAX_USAGES];
    int axisCount;

    // DPAD
    DpadMapping dpads[4];     // fixed 4 directions
    int dpadCount;
    int hatCapIndex;

    // Useful for the hidProfile Lookup
    uint16_t vendorID;
    uint16_t productID;

    // HID info
    HANDLE device;
    PHIDP_PREPARSED_DATA preparsed;
    HIDP_CAPS caps;
    HIDP_BUTTON_CAPS *buttonCaps;
    USHORT buttonCapCount;
    HIDP_VALUE_CAPS *valueCaps;
    USHORT valueCapCount;

    GamepadState *state;
} HidRecord;

void rawInit(void);
void rawUpdate(void);
const GamepadState *rawinput_get_gamepad(int index);