#ifndef JOVIM_H
#define JOVIM_H

#include "common.h"

void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);

#endif
