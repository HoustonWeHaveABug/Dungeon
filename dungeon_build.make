DUNGEON_BUILD_C_FLAGS=-c -O2 -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual -Wconversion -Wformat=2 -Winline -Wlong-long -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Wno-import -Wpointer-arith -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wswitch -Wwrite-strings

dungeon_build.exe: dungeon_build.o
	gcc -o dungeon_build.exe dungeon_build.o

dungeon_build.o: dungeon_build.c dungeon_build.make
	gcc ${DUNGEON_BUILD_C_FLAGS} -o dungeon_build.o dungeon_build.c
