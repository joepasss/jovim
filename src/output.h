#ifndef JOVIM_OUTPUT_H
#define JOVIM_OUTPUT_H

#include "common.h"

void editorRefreshScreen(struct editorConfig *E);
void editorDrawRows(struct abuf *ab, int rows, int cols);
int getWindowSize(int *rows, int *cols);

#endif
