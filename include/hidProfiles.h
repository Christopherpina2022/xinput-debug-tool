#pragma once
#include <stdint.h>
#include <RawInput_Backend.h>
#include <stdio.h>
#include <direct.h>
#include <stdlib.h>

// HID Usage Pages
#define HID_USAGE_PAGE_GENERIC_DESKTOP  0x01

// Generic Desktop Usage IDs
#define HID_USAGE_X                     0x30
#define HID_USAGE_Y                     0x31
#define HID_USAGE_Z                     0x32
#define HID_USAGE_RX                    0x33
#define HID_USAGE_RY                    0x34
#define HID_USAGE_RZ                    0x35
#define HID_USAGE_HAT_SWITCH            0x39

void buildHIDMap(HidRecord *dev);