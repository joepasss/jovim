#ifndef JOVIM_INPUT_H
#define JOVIM_INPUT_H

#include "common.h"

#define CTRL_KEY(k) ((k) & 0x1f)

char editorReadKey();
void editorProcessKeyPress();

#endif
