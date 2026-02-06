#pragma once
#include <input.h>
#include <windows.h>
#include <math.h>
#include <xinput.h>

void xinput_init();
void xinput_update();
const GamepadState *xinput_get_gamepad(int index);