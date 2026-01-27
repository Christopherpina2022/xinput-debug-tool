#pragma once
#include <input.h>

void rawInit(void);
void rawUpdate(void);
const GamepadState *rawinput_get_gamepad(int index);