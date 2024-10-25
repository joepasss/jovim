#ifndef JOVIM_H
#define JOVIM_H

/*** include ***/

/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f)

/*** prototypes ***/

/* terminal */
void die(const char *s);
void enableRawMode();
void disableRawMode();
int getWindowSize(int *rows, int *cols);

/* output */
void editorRefreshScreen();
void editorDrawRows();

/* input */
char editorReadKey();
void editorProcessKeyPress();

#endif
