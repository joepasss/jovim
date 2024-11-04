#include "jovim_error.h"

void die(const char *s) {
  perror(s);
  exit(1);
}
