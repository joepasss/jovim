#ifndef JOVIM_COMMON_H
#define JOVIM_COMMON_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/types.h>

#define _BSD_SOURCE

typedef struct erow {
  int size;
  char *chars;
} erow;

struct editorConfig {
  int cx, cy;
  int screenrows;
  int screencols;
  int numrows;
  erow row;
  struct termios orig_termios;
};

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

#define JOVIM_VERSION "0.0.1"

#endif
