debug: src/main.c
	gcc -g src/main.c src/rawInput.c src/input.c -lxinput -Iinclude -o cdebug -mconsole -lhid
release: src/main.c
	gcc src/main.c src/XInput.c src/input.c -lxinput -Iinclude -O3 cdebug