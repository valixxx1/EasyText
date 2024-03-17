#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <termsize.h>
#include <types.h>

u32 termwidth(void)
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

u32 termheight(void)
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_row;
}
