#include"macro.h"

void set_console_mode_text() {
	int rs = _setmode(_fileno(stdout), _O_TEXT);
}

void set_console_mode_wtext() {
	int rs = _setmode(_fileno(stdout), _O_WTEXT);
}