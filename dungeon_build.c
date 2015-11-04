#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned long erand(unsigned long);

int main(void) {
int *blocks, walls, *types;
unsigned long height, length, width, wall1, wall2, monster, down, up, empty, surface, blocks_n, not_empty, types_n, n, r, i, j, k;
	scanf("%lu", &height);
	if (!height) {
		fprintf(stderr, "Number of levels must be greater than 0\n");
		return EXIT_FAILURE;
	}
	scanf("%lu", &length);
	if (!length) {
		fprintf(stderr, "Level length must be greater than 0\n");
		return EXIT_FAILURE;
	}
	scanf("%lu", &width);
	if (!width) {
		fprintf(stderr, "Level width must be greater than 0\n");
		return EXIT_FAILURE;
	}
	scanf("%lu", &wall1);
	if (wall1 > 100) {
		fprintf(stderr, "Wall start must be lower than or equal to 100\n");
		return EXIT_FAILURE;
	}
	scanf("%lu", &wall2);
	if (wall2 > 100) {
		fprintf(stderr, "Wall row must be lower than or equal to 100\n");
		return EXIT_FAILURE;
	}
	surface = width*length;
	blocks_n = surface*height;
	blocks = malloc(sizeof(int)*blocks_n);
	if (!blocks) {
		fprintf(stderr, "Could not allocate blocks\n");
		return EXIT_FAILURE;
	}
	srand((unsigned)time(NULL));
	n = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < length; j++) {
			for (k = 0; k < width; k++, n++) {
				blocks[n] = '.';
				walls = 0;
				if (j && k && blocks[n-width-1] == '#') {
					walls += 4;
				}
				if (j && blocks[n-width] == '#') {
					walls += 2;
				}
				if (k && blocks[n-1] == '#') {
					walls++;
				}
				if (walls == 3) {
					blocks[n] = '#';
				}
				else if (walls != 4 && walls != 7) {
					r = erand(100);
					if (walls) {
						if (r < wall2) {
							blocks[n] = '#';
						}
					}
					else {
						if (r < wall1) {
							blocks[n] = '#';
						}
					}
				}
			}
		}
	}
	scanf("%lu%lu%lu%lu", &monster, &down, &up, &empty);
	not_empty = monster+down+up;
	types_n = not_empty+empty;
	types = malloc(sizeof(int)*types_n);
	if (!types) {
		fprintf(stderr, "Could not allocate types\n");
		free(blocks);
		return EXIT_FAILURE;
	}
	n = 0;
	for (i = 0; i < monster; i++, n++) {
		types[n] = 'm';
	}
	for (i = 0; i < down; i++, n++) {
		types[n] = 'd';
	}
	for (i = 0; i < up; i++, n++) {
		types[n] = 'u';
	}
	n = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < surface; j++, n++) {
			if (blocks[n] == '.') {
				r = erand(types_n);
				if (r < not_empty) {
					switch (types[r]) {
					case 'm':
						blocks[n] = 'm';
						break;
					case 'd':
						if (i < height-1 && blocks[n+surface] != '#' && blocks[n+surface] != 'u') {
							blocks[n] = 'd';
						}
						break;
					case 'u':
						if (i && blocks[n-surface] != '#' && blocks[n-surface] != 'd') {
							blocks[n] = 'u';
						}
					}
				}
			}
		}
	}
	printf("%lu %lu %lu 0\n", height, length, width);
	n = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < length; j++) {
			for (k = 0; k < width; k++, n++) {
				putchar(blocks[n]);
			}
			puts("");
		}
		if (i < height-1) {
			puts("");
		}
	}
	free(types);
	free(blocks);
	return EXIT_SUCCESS;
}

unsigned long erand(unsigned long values) {
	return (unsigned long)(rand()/(RAND_MAX+1.0)*values);
}
