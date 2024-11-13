#include "input.h"
#include "jovim_error.h"

int editorReadKey() {
  int nread;
  char c;

  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) {
      die("read");
    }
  }

  if (c == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) {
      return '\x1b';
    }

    if (read(STDIN_FILENO, &seq[1], 1) != 1) {
      return '\x1b';
    }

    if (seq[0] == '[') {
      switch (seq[1]) {
      case 'A':
        return ARROW_UP;
      case 'B':
        return ARROW_DOWN;
      case 'C':
        return ARROW_RIGHT;
      case 'D':
        return ARROW_LEFT;
      }
    }

    return '\x1b';
  } else {
    return c;
  }

  return c;
}

void editorMoveCursor(int key, struct editorConfig *E) {
  switch (key) {
  case ARROW_LEFT:
    if (E->cx != 0) {
      E->cx--;
    }
    break;
  case ARROW_RIGHT:
    if (E->cx != E->screencols - 1) {
      E->cx++;
    }
    break;
  case ARROW_UP:
    if (E->cy != 0) {
      E->cy--;
    }
    break;
  case ARROW_DOWN:
    if (E->cy != E->screenrows - 1) {
      E->cy++;
    }
    break;
  }
}

void editorProcessKeyPress(struct editorConfig *E) {
  int c = editorReadKey();

  switch (c) {
  case 'q':
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    exit(0);
    break;

  case ARROW_UP:
  case ARROW_DOWN:
  case ARROW_LEFT:
  case ARROW_RIGHT:
    editorMoveCursor(c, E);
    break;
  }
}
