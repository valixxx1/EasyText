#include <stdnoreturn.h>
#include <termsize.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <types.h>

noreturn void stop() {
  refresh();
  getch();
  endwin();
  exit(0);
}

noreturn void print_err(const char *str, const u64 len) {
  chtype ch_err;
  for (u64 i = 0; i < len-1; i++) {
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
  init_pair(1, COLOR_RED, COLOR_BLACK); /* color of error */
}

void reading(FILE *file)
{
  char ch = fgetc(file);
  u32 countoflines = 1;
  while (ch != EOF) {
    if (countoflines > termheight()) {
      break;
    }
    addch(ch);
    if (ch == '\n') {
      countoflines++;
    }
    ch = fgetc(file);
  }
}

int main(int argc, char **argv) {
  FILE *file = fopen(argv[1], "r");
  init();
  if (!file)
    file_not_exist_err();
  reading(file);
  fclose(file);
  stop();
}
