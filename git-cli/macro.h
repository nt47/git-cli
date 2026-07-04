#pragma once
#include <iostream>
#include <io.h>
#include <fcntl.h>

void set_console_mode_text();
void set_console_mode_wtext();

#define A_START  set_console_mode_text();
#define A_STOP   std::cout.flush();;

#define W_START  set_console_mode_wtext();
#define W_STOP    std::wcout.flush();;