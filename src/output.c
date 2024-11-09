#include "output.h"
#include "jovim.h"
#include "welcome.h"

void editorRefreshScreen(struct editorConfig *E) {
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);

  editorDrawRows(&ab, E->screenrows, E->screencols);

  abAppend(&ab, "\x1b[H", 3);
  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorDrawRows(struct abuf *ab, int rows, int cols) {
  int y;

  for (y = 0; y < rows; y++) {

    if (y == rows / 3) {
      welcomeMessage(ab, cols);
    } else {
      abAppend(ab, "~", 1);
    }

    abAppend(ab, "\x1b[K", 3);

    if (y < rows - 1) {
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
