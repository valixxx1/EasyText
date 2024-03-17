easytext: main.c termsize.c
	gcc -o easytext main.c termsize.c -Iinclude -lncurses
