#ifndef JOVIM_OUTPUT_H
#define JOVIM_OUTPUT_H

#include "common.h"

void editorRefreshScreen(struct editorConfig E);
void editorDrawRows(struct abuf *ab, struct editorConfig E);
int getWindowSize(int *rows, int *cols);

#endif
