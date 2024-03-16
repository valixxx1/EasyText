#include <stdnoreturn.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8 ;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8 ;

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
  init_pair(1, COLOR_RED, COLOR_BLACK);
}

void reading(FILE *file)
{
  char ch = fgetc(file);
  while (ch != EOF) {
    addch(ch);
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
