#ifndef JOVIM_INPUT_H
#define JOVIM_INPUT_H

#include "common.h"

#define CTRL_KEY(k) ((k) & 0x1f)

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  PAGE_UP,
  PAGE_DOWN
};

int editorReadKey();
void editorMoveCursor(int key, struct editorConfig *E);
void editorProcessKeyPress(struct editorConfig *E);

#endif
