#include <stdnoreturn.h>
#include <termsize.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <types.h>
#include <vector>

[[noreturn]] void stop() {
  refresh();
  endwin();
  exit(0);
}

[[noreturn]] void print_err(const char *str, const u64 len) {
  chtype ch_err;
  for (u64 i = 0; i < len-1; i++) {
    ch_err = str[i] | COLOR_PAIR(1);
    addch(ch_err);
  }
  getch();
  stop();
}

[[noreturn]] void file_not_exist_err() {
  char str[] = "File does not exist!";
  print_err(str, sizeof(str));
}

void init() {
  initscr();
  move(0, 0);
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK); /* color of error */
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}

void findnewlines(FILE *file, std::vector<u64> &newlines)
{
  char ch = fgetc(file);
  while (ch != EOF) {
    if (ch == '\n') {
      newlines.push_back(ftell(file));
    }
    ch = fgetc(file);
  }
  fseek(file, SEEK_SET, 0);
}

/* firstline is a line that is on the top of screen */
void reading(FILE *file, std::vector<u64> &newlines, u64 firstline)
{
  if (firstline == 0) {
    fseek(file, 0, SEEK_SET);
  } else {
    fseek(file, newlines[firstline-1], SEEK_SET);
  }
  char ch = fgetc(file);
  u64 countoflines = 1;
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
  refresh();
}

int main(int argc, char **argv) {
  FILE *file = fopen(argv[1], "r");
  int ch; /* code of current symbol */
  int curline = 0;
  std::vector<u64> newlines; /* positions of every newline character */
  init();
  if (!file)
    file_not_exist_err();
  findnewlines(file, newlines);
  reading(file, newlines, curline);

  for (;;) {
    ch = getch();
    move(0, 0);
    switch (ch) {
      case 27: /* Esc */
        goto exit;
      case KEY_UP:
        if (curline > 0) {
          reading(file, newlines, --curline);
        }
        break;
      case KEY_DOWN:
        if (curline + termheight() < newlines.size()) {
          reading(file, newlines, ++curline);
        }
        break;
      default:
        reading(file, newlines, curline);
        break;
    }
  }

exit:
  fclose(file);
  stop();
}
