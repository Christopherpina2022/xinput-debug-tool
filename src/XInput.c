#include <xinput_Backend.h>

// User defined
#define INPUT_DEADZONE 0.15f

static GamepadState controllers[MAX_CONTROLLERS];

void xinput_init() {
    memset(controllers, 0, sizeof(controllers));
}

// Convert raw XInput value to [-1,1] for sticks or [0,1] for triggers,
// then apply deadzone.
static float applyDeadzoneNormalized(float rawValue, float deadzone, int isTrigger) {
    float normalized = 0.0f;

    if (isTrigger) {
        // Triggers are BYTE: 0..255
        normalized = rawValue / 255.0f;
        if (normalized < deadzone) return 0.0f;
        // Rescale remaining range to full 0..1
        return (normalized - deadzone) / (1.0f - deadzone);
    } else {
        // Sticks are SHORT: -32768..32767
        if (rawValue >= 0)
            normalized = rawValue / 32767.0f;
        else
            normalized = rawValue / 32768.0f;

        if (fabsf(normalized) < deadzone) return 0.0f;
        // Rescale remaining range to full -1..1
        if (normalized > 0.0f)
            return (normalized - deadzone) / (1.0f - deadzone);
        else
            return (normalized + deadzone) / (1.0f - deadzone);
    }
}

static uint16_t mapButtons(WORD buttons) {
    uint16_t out = 0;

    if (buttons & XINPUT_GAMEPAD_A)              out |= BTN_A;
    if (buttons & XINPUT_GAMEPAD_B)              out |= BTN_B;
    if (buttons & XINPUT_GAMEPAD_X)              out |= BTN_X;
    if (buttons & XINPUT_GAMEPAD_Y)              out |= BTN_Y;

    if (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)  out |= BTN_LB;
    if (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) out |= BTN_RB;

    if (buttons & XINPUT_GAMEPAD_BACK)           out |= BTN_BACK;
    if (buttons & XINPUT_GAMEPAD_START)          out |= BTN_START;

    if (buttons & XINPUT_GAMEPAD_LEFT_THUMB)     out |= BTN_LS;
    if (buttons & XINPUT_GAMEPAD_RIGHT_THUMB)    out |= BTN_RS;

    if (buttons & XINPUT_GAMEPAD_DPAD_UP)        out |= BTN_DPAD_UP;
    if (buttons & XINPUT_GAMEPAD_DPAD_DOWN)      out |= BTN_DPAD_DOWN;
    if (buttons & XINPUT_GAMEPAD_DPAD_LEFT)      out |= BTN_DPAD_LEFT;
    if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT)     out |= BTN_DPAD_RIGHT;

    return out;
}

void xinput_update() {
    for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(state));

        if (XInputGetState(i, &state) == ERROR_SUCCESS) {
            GamepadState *g = &controllers[i];
            g->connected = 1;

            // Axes
            g->axes[INPUT_AXIS_LEFT_X]  = applyDeadzoneNormalized(state.Gamepad.sThumbLX, INPUT_DEADZONE, 0);
            g->axes[INPUT_AXIS_LEFT_Y]  = applyDeadzoneNormalized(state.Gamepad.sThumbLY, INPUT_DEADZONE, 0); // invert Y
            g->axes[INPUT_AXIS_RIGHT_X] = applyDeadzoneNormalized(state.Gamepad.sThumbRX, INPUT_DEADZONE, 0);
            g->axes[INPUT_AXIS_RIGHT_Y] = applyDeadzoneNormalized(state.Gamepad.sThumbRY, INPUT_DEADZONE,0);
            g->axes[INPUT_AXIS_LT]      = applyDeadzoneNormalized(state.Gamepad.bLeftTrigger, INPUT_DEADZONE, 1);
            g->axes[INPUT_AXIS_RT]      = applyDeadzoneNormalized(state.Gamepad.bRightTrigger, INPUT_DEADZONE, 1);

            // Buttons + DPAD
            g->buttons = mapButtons(state.Gamepad.wButtons);
        } else {
            controllers[i].connected = 0;
        }
    }
}

const GamepadState *xinput_get_gamepad(int index) {
    if (index < 0 || index >= MAX_CONTROLLERS) {
        return NULL;
    }
    return &controllers[index];
}