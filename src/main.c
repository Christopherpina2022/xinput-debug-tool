#include <windows.h>
#include <xinput.h>
#include <stdio.h>
#include <math.h>

// Defined by the Microsoft Xinput Documentation page
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30
#define MAGNITUDE_MAX 32767
// User defined, range acceptable is from 0-65534
#define INPUT_DEADZONE 255
// range of console
#define WIDTH 60
#define HEIGHT 12

// Define global variables
char screen[WIDTH * HEIGHT];

void toBuffer (int x, int y, const char *string) {
    int i = 0;
    while (string[i] && x + i < WIDTH) {
        screen[y * WIDTH + x + i] = string[i];
        i++;
    }
}

void renderController(const XINPUT_STATE *state) {
    WORD buttons = state->Gamepad.wButtons;
    char tempBuffer[32];

    toBuffer(0,0, "Controller 0");
    toBuffer(0,1, "============");

    sprintf(tempBuffer, "A: %-8s\n", (XINPUT_GAMEPAD_A & buttons) ? "Pressed" : "Released");
    toBuffer(0,3, tempBuffer);

    sprintf(tempBuffer, "B: %-8s\n", (XINPUT_GAMEPAD_B & buttons) ? "Pressed" : "Released");
    toBuffer(0,4, tempBuffer);

    sprintf(tempBuffer, "X: %-8s\n", (XINPUT_GAMEPAD_X & buttons) ? "Pressed" : "Released");
    toBuffer(0,5, tempBuffer);

    sprintf(tempBuffer, "Y: %-8s\n", (XINPUT_GAMEPAD_Y & buttons) ? "Pressed" : "Released");
    toBuffer(0,6, tempBuffer);

    sprintf(tempBuffer, "    LX: %6d\n", (state->Gamepad.sThumbLX));
    toBuffer(0,8, tempBuffer);

    sprintf(tempBuffer, "    LY: %d\n", (state->Gamepad.sThumbLY));
    toBuffer(0,9, tempBuffer);
}

void flushBuffer(HANDLE hConsole) {
    DWORD written;
    COORD origin = {0,0};

    WriteConsoleOutputCharacterA(
        hConsole,
        screen,
        WIDTH * HEIGHT,
        origin,
        &written
    );
}

void clearRegion () {
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        screen[i] = ' ';
    }
}

char *batteryLevel(BYTE batLevel) {
    switch (batLevel)
    {
    case BATTERY_LEVEL_EMPTY:   return "Empty";
    case BATTERY_LEVEL_LOW:     return "Low";
    case BATTERY_LEVEL_MEDIUM:  return "Medium";
    case BATTERY_LEVEL_FULL:    return "Full";
    default:                    return "Unknown";
    }
}

char *batteryType(BYTE batType) {
    switch (batType)
    {
    case BATTERY_TYPE_WIRED:    return "Wired";
    case BATTERY_TYPE_ALKALINE: return "Alkaline";
    case BATTERY_TYPE_NIMH:     return "Nickel Metal Hydride";
    case BATTERY_TYPE_UNKNOWN:  return "Unknown";
    default:
    }
}

void setDeadzone (float *magnitude, float *normalizedMagnitude) {
    if (*magnitude > INPUT_DEADZONE) {
        if (*magnitude > MAGNITUDE_MAX) *magnitude = MAGNITUDE_MAX;
        *magnitude -= INPUT_DEADZONE;
        *normalizedMagnitude = *magnitude / (MAGNITUDE_MAX - INPUT_DEADZONE);
    }
    else {
        *magnitude = 0.0;
        *normalizedMagnitude = 0.0;
    }
}

int main () {
    // Initialize Structures
    XINPUT_STATE state;
    XINPUT_BATTERY_INFORMATION batteryInfo;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD origin = {0,0};

    // Dead Zone variables (not useful until i get an actual controller instead of this fight stick)
    float LX = state.Gamepad.sThumbLX;
    float LY = state.Gamepad.sThumbLY;
    float magnitude = sqrt(LX * LY + LY * LY);
    float normalizedLX = LX / magnitude;
    float normalizedLY = LY / magnitude;
    float normalizedMagnitude = 0;

    setDeadzone(&magnitude, &normalizedMagnitude);
    system("cls");
    
    while (1) {
        clearRegion(hConsole, WIDTH, HEIGHT);

        // Checks if our controller is plugged in by clearing the memory of it every frame
        ZeroMemory(&state, sizeof(state));
        DWORD result = XInputGetState(0, &state);
        
        if (result == ERROR_SUCCESS)
        {
            // Collects battery info and what buttons are being pressed at that frame
            DWORD batteryResult = XInputGetBatteryInformation(0, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);
            renderController(&state);
        }
        else {
            toBuffer(0, 0, "controller is not plugged in.");
        }

        flushBuffer(hConsole);
        Sleep(16);
    }
}