#ifndef JOVIM_H
#define JOVIM_H

/*** include ***/
#ifndef JOVIM_INCLUDE
#define JOVIM_INCLUDE

#include <termios.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#endif

/*** defines ***/
#define JOVIM_VERSION "0.0.1"
#define JOVIM_WELCOME_ROW_LENGTH 5

#define CTRL_KEY(k) ((k) & 0x1f)
#define ABUF_INIT {NULL, 0}

enum editorKey {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

struct editorConfig {
  int cx, cy;
  int screenrows;
  int screencols;
  struct termios orig_termios;
};

struct abuf {
  char *b;
  int len;
};

/*** prototypes ***/

/* terminal */
void die(const char *s);
void enableRawMode();
void disableRawMode();
int getWindowSize(int *rows, int *cols);
void abAppend(struct abuf *ab, const char *s, int len);
void abFree(struct abuf *ab);

/* output */
void editorRefreshScreen();
void editorDrawRows(struct abuf *ab);
void welcomeMessage(struct abuf *ab);
void authorMessage(struct abuf *ab);
void helpMessage(struct abuf *ab);

/* input */
int editorReadKey();
void editorProcessKeyPress();
void editorMoveCursor(int key);

#endif
