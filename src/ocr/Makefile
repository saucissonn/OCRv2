all:
	gcc -o n $(shell find . -name '*.c') \
		-lSDL2_ttf -lSDL2_image -lSDL2 -lm \
		-fsanitize=address -g
