#ifndef JOVIM_COMMON_H
#define JOVIM_COMMON_H

#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>

struct editorConfig {
  int screenrows;
  int screencols;
  struct termios orig_termios;
};

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

#endif
