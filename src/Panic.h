#pragma once
#include "Interrupts/interrupts.h"

void Panic(const char* message, interrupt_frame* frame);
