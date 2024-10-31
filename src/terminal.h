#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

extern struct termios orig_termios;

void refresh_prompt(void);
void clear_screen(void);
char read_key(void);
void enable_raw_mode(void);
void disable_raw_mode(void);

#endif
