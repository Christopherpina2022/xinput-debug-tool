#include <input.h>
#include <XInput_Backend.h>
#include <RawInput_Backend.h>

typedef enum {
    INPUT_BACKEND_RAW,
    INPUT_BACKEND_XINPUT
} InputBackend;

static InputBackend activeBackend = INPUT_BACKEND_RAW;

static int isXInputConnected() {
    xinput_update(); // update XInput states
    for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
        const GamepadState *pad = xinput_get_gamepad(i);
        if (pad && pad->connected) return 1;
    }
    return 0;
}

void input_init() {
    xinput_init();
    rawInit();

    // Pick backend at startup
    activeBackend = isXInputConnected() ? INPUT_BACKEND_XINPUT : INPUT_BACKEND_RAW;
}

void input_update() {
    // update runs twice to help detect a controller change
    rawUpdate();
    xinput_update();

    if (isXInputConnected()) {
        if (activeBackend != INPUT_BACKEND_XINPUT) {
            activeBackend = INPUT_BACKEND_XINPUT;
        }
    } else {
        if (activeBackend != INPUT_BACKEND_RAW) {
            activeBackend = INPUT_BACKEND_RAW;
        }
    }

    switch (activeBackend)
    {
        case INPUT_BACKEND_RAW:
            rawUpdate();
            break;
        
        case INPUT_BACKEND_XINPUT:
            xinput_update();
            break;
    }
}

const GamepadState *input_get_gamepad(int index) {
    switch (activeBackend)
    {
        case INPUT_BACKEND_RAW:
            return rawinput_get_gamepad(index);
        case INPUT_BACKEND_XINPUT:
            return xinput_get_gamepad(index);
    }
    return NULL;
}