#include "output.h"
#include "jovim.h"
#include "welcome.h"

void editorRefreshScreen(struct editorConfig E) {
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab, E);

  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
  abAppend(&ab, buf, strlen(buf));

  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorDrawRows(struct abuf *ab, struct editorConfig E) {
  int y;

  for (y = 0; y < E.screenrows; y++) {
    if (y >= E.numrows) {
      if (E.numrows == 0 && y == E.screenrows / 3) {
        welcomeMessage(ab, E.screencols);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      int len = E.row.size;

      if (len > E.screencols) {
        len = E.screencols;
      }

      abAppend(ab, E.row.chars, len);
    }

    abAppend(ab, "\x1b[K", 3);

    if (y < E.screenrows - 1) {
      abAppend(ab, "\r\n", 2);
    }
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
