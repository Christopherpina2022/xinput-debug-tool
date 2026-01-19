debug: src/main.c
	gcc -g src/main.c src/XInput.c src/input.c -lxinput -Iinclude -o cdebug
rawdebug: src/rawInput.c
	gcc -g -lxinput -Iinclude -o cdebug
release: src/main.c
	gcc src/main.c src/XInput.c src/input.c -lxinput -Iinclude -O3 cdebug