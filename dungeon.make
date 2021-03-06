DUNGEON_C_FLAGS=-c -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

dungeon.exe: dungeon.o
	gcc -o dungeon.exe dungeon.o

dungeon.o: dungeon.c dungeon.make
	gcc ${DUNGEON_C_FLAGS} -o dungeon.o dungeon.c
