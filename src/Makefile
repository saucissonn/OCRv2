all:
	gcc -o n $(shell find . -name '*.c') \
		-lSDL2_ttf -lSDL2_image -lSDL2 -lm \
		-fsanitize=address,undefined \
		-fno-omit-frame-pointer \
		-g -O0

run:
	LSAN_OPTIONS=suppressions=lsan.supp:print_suppressions=1 ./n
