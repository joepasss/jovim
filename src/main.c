#include "jovim.h"
#include <unistd.h>

struct editorConfig E;

/*** terminal ***/

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios)) {
    die("tcsetattr");
  }
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios)) {
    die("tcgetattr");
  };

  atexit(disableRawMode);

  struct termios raw = E.orig_termios;

  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)) {
    die("tcsetattr");
  }
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;

    return 0;
  }
}

void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL) {
    return;
  }

  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}

void abFree(struct abuf *ab) { free(ab->b); }

/*** output ***/

void editorRefreshScreen() {
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab);

  char buf[32];

  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);

  abAppend(&ab, buf, strlen(buf));
  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorDrawRows(struct abuf *ab) {
  int y;
  int welcomeRow = (E.screenrows - JOVIM_WELCOME_ROW_LENGTH) / 2;
  int authorRow = welcomeRow + 2;
  int helpRow = welcomeRow + 4;

  for (y = 0; y < E.screenrows; y++) {
    if (y == welcomeRow) {
      welcomeMessage(ab);
    } else if (y == authorRow) {
      authorMessage(ab);
    } else if (y == helpRow) {
      helpMessage(ab);
    } else {
      abAppend(ab, "~", 1);
    }

    abAppend(ab, "\x1b[K", 3);

    if (y < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
  }
}

void centerText(struct abuf *ab, char *str, int strlen) {
  int padding = (E.screencols - strlen) / 2;

  if (padding) {
    abAppend(ab, "~", 1);
    padding--;
  }

  while (padding--) {
    abAppend(ab, " ", 1);
  }

  abAppend(ab, str, strlen);
}

void welcomeMessage(struct abuf *ab) {
  char welcome[80];
  int welcomelen =
      snprintf(welcome, sizeof(welcome), "JOVIM v%s", JOVIM_VERSION);

  if (welcomelen > E.screencols) {
    welcomelen = E.screencols;
  }

  centerText(ab, welcome, welcomelen);
}

void authorMessage(struct abuf *ab) {
  char author[80];
  int authorlen = snprintf(author, sizeof(author), "by JOEPASSS");

  if (authorlen > E.screencols) {
    authorlen = E.screencols;
  }

  centerText(ab, author, authorlen);
}

void helpMessage(struct abuf *ab) {
  char help[80];
  int helplen = snprintf(help, sizeof(help), "type  q       to exit");

  if (helplen > E.screencols) {
    helplen = E.screencols;
  }

  centerText(ab, help, helplen);
}

/*** input ***/

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
}

void editorProcessKeyPress() {
  int c = editorReadKey();

  switch (c) {
  case 'q':
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    exit(0);
    break;

  case ARROW_LEFT:
  case ARROW_RIGHT:
  case ARROW_UP:
  case ARROW_DOWN:
    editorMoveCursor(c);
    break;
  }
}

void editorMoveCursor(int key) {
  switch (key) {
  case ARROW_LEFT:
    if (E.cx != 0) {
      E.cx--;
    }
    break;
  case ARROW_RIGHT:
    if (E.cx != E.screencols - 1) {
      E.cx++;
    }
    break;
  case ARROW_UP:
    if (E.cy != 0) {
      E.cy--;
    }
    break;
  case ARROW_DOWN:
    if (E.cy != E.screenrows - 1) {
      E.cy++;
    }
    break;
  }
}

int main() {
  enableRawMode();

  // init
  E.cx = 0;
  E.cy = 0;

  if (getWindowSize(&E.screenrows, &E.screencols) == -1) {
    die("getWindowSize");
  }

  while (1) {
    editorRefreshScreen();
    editorProcessKeyPress();
  }

  return 0;
}
