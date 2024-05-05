#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termio.h>
#include <termios.h>
#include <unistd.h>

struct termios org_terminal;

void die(const char *msg) {
  perror(msg);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &org_terminal) == -1) {
    die("disable raw");
  }
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &org_terminal) == -1) {
    die("enable raw mode");
  }

  struct termios raw = org_terminal;
  atexit(disableRawMode);
  raw.c_oflag &= ~(OPOST);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("enable raw mode");
  }
}

int main() {
  if (tcgetattr(STDIN_FILENO, &org_terminal) == -1) {
    die("get tc");
  }

  enableRawMode();
  char c;
  while (1) {
    c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      die("read");
    }

    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') {
      break;
    }
  }
  return 0;
}
