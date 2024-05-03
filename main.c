#include <stdlib.h>
#include <termio.h>
#include <termios.h>
#include <unistd.h>

struct termios org_terminal;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_terminal); }

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &org_terminal);

  struct termios raw = org_terminal;
  atexit(disableRawMode);
  raw.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  tcgetattr(STDIN_FILENO, &org_terminal);
  enableRawMode();
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    ;

  return 0;
}
