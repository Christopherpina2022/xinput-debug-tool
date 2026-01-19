#pragma once
#include "input.h"

void xinput_init(void);
void xinput_update(void);
const GamepadState* xinput_get_gamepad(int index);