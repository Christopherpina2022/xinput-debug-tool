#include <xinput_Backend.h>
#include <input.h>
#include <math.h>
#include <windows.h>
#include <xinput.h>

// Defined by the Microsoft Xinput Documentation page
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30
#define MAGNITUDE_MAX 32767
// User defined, range acceptable is from 0-65534
#define INPUT_DEADZONE 50.0f

static GamepadState controllers[MAX_CONTROLLERS];

float setDeadzone (XINPUT_STATE *state) {
    // Dead Zone variables (not useful until i get an actual controller instead of this fight stick)
    float LX = state->Gamepad.sThumbLX;
    float LY = state->Gamepad.sThumbLY;
    float normalizedLX = 0.0f;
    float normalizedLY = 0.0f;
    float magnitude = sqrt(LX * LX + LY * LY);

    // prevents a divide by zero error when the stick is idle
    if (magnitude > 0.0f) {
        normalizedLX = LX / magnitude;
        normalizedLY = LY / magnitude;
    }
}

void xinput_init() {
    memset(controllers, 0, sizeof(controllers));
}

// Normalize stick axes to [-1,1], triggers to [0,1]
static float normalizeAxis(SHORT value, SHORT deadzone) {
    if (value > -deadzone && value < deadzone)
        return 0.0f;

    float normalized = (float)value / 32767.0f;
    if (normalized > 1.0f) normalized = 1.0f;
    if (normalized < -1.0f) normalized = -1.0f;
    return normalized;
}

static float normalizeTrigger(BYTE value) {
    if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) return 0.0f;
    return (float)value / 255.0f;
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
            g->axes[INPUT_AXIS_LEFT_X]  = normalizeAxis(state.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            g->axes[INPUT_AXIS_LEFT_Y]  = normalizeAxis(state.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            g->axes[INPUT_AXIS_RIGHT_X] = normalizeAxis(state.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            g->axes[INPUT_AXIS_RIGHT_Y] = normalizeAxis(state.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
            g->axes[INPUT_AXIS_LT]      = normalizeTrigger(state.Gamepad.bLeftTrigger);
            g->axes[INPUT_AXIS_RT]      = normalizeTrigger(state.Gamepad.bRightTrigger);

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