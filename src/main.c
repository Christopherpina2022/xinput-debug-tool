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

void printControllerData(WORD buttons, XINPUT_STATE *state) {
    printf("Controller 0\n");
    printf("============\n");
    
    printf("Buttons: 0\n");

    printf("    A: %s\n", (XINPUT_GAMEPAD_A & buttons) ? "Pressed" : "Released");
    printf("    B: %s\n", (XINPUT_GAMEPAD_B & buttons) ? "Pressed" : "Released");
    printf("    X: %s\n", (XINPUT_GAMEPAD_X & buttons) ? "Pressed" : "Released");
    printf("    Y: %s\n", (XINPUT_GAMEPAD_Y & buttons) ? "Pressed" : "Released");

    printf("Left Thumb Stick\n");

    printf("    LX: %d\n", (state->Gamepad.sThumbLX));
    printf("    LY: %d\n", (state->Gamepad.sThumbLY));
}

void hideCursor(HANDLE hConsole) {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}

void clearRegion (HANDLE hConsole, SHORT width, SHORT height) {
    DWORD written;
    COORD origin = {0,0};

    for (SHORT y = 0; y<height; y++) {
        COORD line = {0, y};
        FillConsoleOutputCharacterA(hConsole, ' ', width, line, &written);
    }

    SetConsoleCursorPosition(hConsole, origin);
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

    // Clear the Screen
    hideCursor(hConsole);
    
    while (1) {
        clearRegion(hConsole, WIDTH, HEIGHT);

        // Checks if our controller is plugged in by clearing the memory of it every frame
        ZeroMemory(&state, sizeof(state));
        DWORD result = XInputGetState(0, &state);
        
        if (result == ERROR_SUCCESS)
        {
            // Collects battery info and what buttons are being pressed at that frame
            DWORD batteryResult = XInputGetBatteryInformation(0, BATTERY_DEVTYPE_GAMEPAD, &batteryInfo);
            WORD buttons = state.Gamepad.wButtons;
            // Dead Zone compensation
            if (magnitude > INPUT_DEADZONE) {
                if (magnitude > MAGNITUDE_MAX) magnitude = MAGNITUDE_MAX;
                magnitude -= INPUT_DEADZONE;
                normalizedMagnitude = magnitude / (MAGNITUDE_MAX - INPUT_DEADZONE);
            }
            else {
                magnitude = 0.0;
                normalizedMagnitude = 0.0;
            }

            printControllerData(buttons, &state);
        }
        else {
            printf("controller is not plugged in.");

            fflush(stdout);
            Sleep(64);
        }
    }
}