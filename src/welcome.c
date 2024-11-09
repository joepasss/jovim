#include "welcome.h"
#include "common.h"
#include "jovim.h"

void welcomeMessage(struct abuf *ab, int screencols) {
  char welcome[80];

  int welcomelen =
      snprintf(welcome, sizeof(welcome), "JOVIM --version %s", JOVIM_VERSION);

  if (welcomelen > screencols) {
    welcomelen = screencols;
  }

  int padding = (screencols - welcomelen) / 2;

  if (padding) {
    abAppend(ab, "~", 1);
  }

  while (padding--) {
    abAppend(ab, " ", 1);
  }

  abAppend(ab, welcome, welcomelen);
}
