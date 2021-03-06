DUNGEON_DEBUG_C_FLAGS=-c -g -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

dungeon_debug.exe: dungeon_debug.o
	gcc -g -o dungeon_debug.exe dungeon_debug.o

dungeon_debug.o: dungeon.c dungeon_debug.make
	gcc ${DUNGEON_DEBUG_C_FLAGS} -o dungeon_debug.o dungeon.c
