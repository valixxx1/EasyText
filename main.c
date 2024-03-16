#include <ncurses.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <stdlib.h>

noreturn void stop() {
  refresh();
  getch();
  endwin();
  exit(0);
}

noreturn void print_err(const char *str, const size_t len) {
  chtype ch_err;
  for (int i = 0; i < len-1; i++) {
    ch_err = str[i] | COLOR_PAIR(1);
    addch(ch_err);
  }
  stop();
}

noreturn void file_not_exist_err() {
  char str[] = "File does not exist!";
  print_err(str, sizeof(str));
}

void init() {
  initscr();
  move(0, 0);
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
}

int main(int argc, char **argv) {
  FILE *file;
  char ch;
  init();

  if ((file = fopen(argv[1], "r")) == NULL) {
    file_not_exist_err();
  }

  ch = fgetc(file);
  while (ch != EOF) {
    addch(ch);
    ch = fgetc(file);
  }

  fclose(file);
  stop();
}
