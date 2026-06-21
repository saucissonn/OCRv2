all:
	gcc -o n *.c -lSDL2_ttf -lSDL2_image -lSDL2 -lm -fsanitize=address -g
