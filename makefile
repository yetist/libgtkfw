all: test-2.0 window

test-2.0: main.c magic-button.c
	gcc -g -o test-2.0 main.c magic-button.c magic-button.h `pkg-config --cflags --libs gtk+-2.0`

test-3.0: main.c magic-button.c
	gcc -g -o test-3.0 main.c magic-button.c magic-button.h `pkg-config --cflags --libs gtk+-3.0`

window: a.c magic-window.c
	gcc -o window magic-window.c  magic-button.c a.c `pkg-config --cflags --libs gtk+-2.0`

