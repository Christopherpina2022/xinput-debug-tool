#include <hidProfiles.h>

void applyProfile(HidRecord *dev) {
    if (!dev) return;

    switch (dev->vendorID) {
        case SONY_VID:
            /*  We will only map by VID for now becasue we have to map for literally
                every edge case, and I don't wanna do that*/

            // Buttons
            dev->buttonMap[1] = 1; // Cross
            dev->buttonMap[2] = 0; // Circle
            dev->buttonMap[3] = 2; // Square
            dev->buttonMap[4] = 3; // Triangle

            dev->buttonMap[5] = 4; // L1
            dev->buttonMap[6] = 5; // R1

            dev->buttonMap[7] = 6; // SELECT
            dev->buttonMap[8] = 7; // START

            dev->buttonMap[9] = 8; // L3
            dev->buttonMap[10] = 9; // R3

            dev->buttonMap[11] = 10; // DUP
            dev->buttonMap[12] = 11; // DDOWN
            dev->buttonMap[13] = 12; // DLEFT
            dev->buttonMap[14] = 13; // DRIGHT

            // Axes
            dev->axisMap[0x30] = AXIS_LX;
            dev->axisMap[0x31] = AXIS_LY;
            dev->axisMap[0x32] = AXIS_RX;
            dev->axisMap[0x33] = AXIS_RY;
            break;
        case NINTENDO_VID:
            // Not testing for now (no controller T_T)
            break;
        default:
            // Use Xbox config for generic controllers

            // Buttons
            dev->buttonMap[1] = 0; // Cross
            dev->buttonMap[2] = 1; // Circle
            dev->buttonMap[3] = 2; // Square
            dev->buttonMap[4] = 3; // Triangle

            dev->buttonMap[5] = 4; // L1
            dev->buttonMap[6] = 5; // R1

            dev->buttonMap[7] = 6; // SELECT
            dev->buttonMap[8] = 7; // START

            dev->buttonMap[9] = 8; // L3
            dev->buttonMap[10] = 9; // R3

            dev->buttonMap[11] = 10; // DUP
            dev->buttonMap[12] = 11; // DDOWN
            dev->buttonMap[13] = 12; // DLEFT
            dev->buttonMap[14] = 13; // DRIGHT

            // Axes
            dev->axisMap[0x30] = AXIS_LX;
            dev->axisMap[0x31] = AXIS_LY;
            dev->axisMap[0x32] = AXIS_RX;
            dev->axisMap[0x33] = AXIS_RY;
            break;
    }
}