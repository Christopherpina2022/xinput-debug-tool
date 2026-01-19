#include <input.h>
#include <XInput_Backend.h>

void input_init() {
    xinput_init();
}

void input_update() {
    xinput_update();
}

const GamepadState *input_get_gamepad(int index) {
    return xinput_get_gamepad(index);
}