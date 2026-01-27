#include <input.h>
#include <XInput_Backend.h>
#include <RawInput_Backend.h>

typedef enum {
    INPUT_BACKEND_RAW,
    INPUT_BACKEND_XINPUT
} InputBackend;

static InputBackend activeBackend = INPUT_BACKEND_RAW;

/* All reference to Xinput will be disabled on frontend until I
refactor it for the new Frontend*/

void input_init() {
    //xinput_init();
    rawInit();

    activeBackend = INPUT_BACKEND_RAW;
}

void input_update() {
    switch (activeBackend)
    {
        case INPUT_BACKEND_RAW:
            rawUpdate();
            break;
        
        case INPUT_BACKEND_XINPUT:
            //xinput_update();
            break;
    }
}

const GamepadState *input_get_gamepad(int index) {
    switch (activeBackend)
    {
        case INPUT_BACKEND_RAW:
            return rawinput_get_gamepad(index);
        case INPUT_BACKEND_XINPUT:
            //return xinput_get_gamepad(index);
            return NULL;
    }
    return NULL;
}