debug: src/main.c
	gcc -g src/main.c -lxinput -o xinput
release: main.c
	gcc src/main.c -lxinput -O3 xinput