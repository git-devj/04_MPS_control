#pragma once
#include <iostream>
#include <conio.h>
#include <string.h>
#include <time.h>

#include "ctype.h"
#include "hardware.h"

extern unsigned short output[80];
extern unsigned short input[80];

#define KEY_ENTER 13

#define LONG_MAX 4294957296;

#define COMMAND_MAX 64

void MpsUpdate();
