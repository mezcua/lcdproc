#ifndef MODE_H
#define MODE_H

#include "main.h"


int mode_init(void);
void mode_close(void);

int update_screen(mode * m, int display);

int credit_screen(int rep, int display, int *flags_ptr);

#endif
