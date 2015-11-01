#include <stdio.h>
#include <stdlib.h>

typedef struct cell_s cell_t;
struct cell_s {
	int type;
	unsigned long power;
	int visited;
	unsigned long full_extra;
	unsigned long extra;
	cell_t *from;
	unsigned long links_n;
	cell_t **links;
};

unsigned long cell_index(unsigned long, unsigned long, unsigned long);
int read_cell(cell_t *, unsigned long, unsigned long, unsigned long, int);
int level_links(cell_t *, cell_t *);
int side_links(cell_t *, cell_t *);
int side_link(cell_t *, cell_t *);
int add_link(cell_t *, cell_t *);
int search_path(const char *, cell_t *, cell_t *);
void add_to_queue(cell_t *);
void test_link(cell_t *, cell_t *);
void reset_cell(cell_t *);
void free_cells(unsigned long);

unsigned long levels_n, length, width, power, level_size, queue_size;
cell_t *cells, *start, *goal, **queue;

int main(void) {
unsigned long cells_n, n, i, j, k;
cell_t *cell;
	scanf("%lu", &levels_n);
	if (!levels_n) {
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
	scanf("%lu", &power);
	while (fgetc(stdin) != '\n');
	level_size = length*width;
	cells_n = levels_n*level_size;
	cells = malloc(sizeof(cell_t)*cells_n);
	if (!cells) {
		fprintf(stderr, "Could not allocate cells\n");
		return EXIT_FAILURE;
	}
	start = NULL;
	goal = NULL;
	queue_size = 0;
	for (i = 0; i < levels_n; i++) {
		for (j = 0; j < length; j++) {
			for (k = 0; k < width; k++) {
				n = cell_index(i, j, k);
				if (read_cell(&cells[n], i, j, k, fgetc(stdin))) {
					free_cells(n);
					return EXIT_FAILURE;
				}
			}
			while (fgetc(stdin) != '\n');
		}
		if (i < levels_n-1) {
			while (fgetc(stdin) != '\n');
		}
	}
	queue = malloc(sizeof(cell_t *)*queue_size);
	if (!queue) {
		fprintf(stderr, "Could not allocate queue\n");
		return EXIT_FAILURE;
	}
	if (!search_path("THE QUEST", start, goal)) {
		for (cell = goal; cell; cell = cell->from) {
			if (cell->type == '*') {
				cell->type = ' ';
			}
			else if (cell->type == 'M') {
				cell->type = ' ';
				cell->full_extra = 0;
			}
		}
		for (i = 0; i < queue_size; i++) {
			reset_cell(queue[i]);
		}
		search_path("THE WAY BACK", goal, start);
	}
	free(queue);
	free_cells(cells_n);
	return EXIT_SUCCESS;
}

unsigned long cell_index(unsigned long level, unsigned long y, unsigned long x) {
	return level*level_size+y*width+x;
}

int read_cell(cell_t *cell, unsigned long level, unsigned long y, unsigned long x, int type) {
	cell->type = type;
	switch (cell->type) {
	case 'S':
		if (start) {
			fprintf(stderr, "Start already set\n");
			return 1;
		}
		else {
			start = cell;
			cell->power = power;
		}
		break;
	case 'G':
		if (goal) {
			fprintf(stderr, "Goal already set\n");
			return 1;
		}
		else {
			goal = cell;
		}
		break;
	case ' ':
	case 'M':
	case 'D':
	case 'U':
	case '#':
		break;
	default:
		fprintf(stderr, "Invalid type\n");
		return 1;
	}
	if (cell->type != '#') {
		cell->visited = 0;
		cell->full_extra = cell->type == 'M' ? level+1:0;
		cell->extra = cell->full_extra;
		cell->links_n = 0;
		if (level && level_links(&cells[cell_index(level-1, y, x)], cell)) {
			return 1;
		}
		if (y && side_links(&cells[cell_index(level, y-1, x)], cell)) {
			return 1;
		}
		if (x && side_links(&cells[cell_index(level, y, x-1)], cell)) {
			return 1;
		}
		queue_size += 1+cell->full_extra;
	}
	return 0;
}

int level_links(cell_t *cell1, cell_t *cell2) {
	if (cell1->type == 'D' && add_link(cell1, cell2)) {
		return 1;
	}
	if (cell1->type != '#' && cell2->type == 'U' && add_link(cell2, cell1)) {
		return 1;
	}
	return 0;
}

int side_links(cell_t *cell1, cell_t *cell2) {
	if (side_link(cell1, cell2)) {
		return 1;
	}
	if (cell1->type != '#' && side_link(cell2, cell1)) {
		return 1;
	}
	return 0;
}

int side_link(cell_t *cell1, cell_t *cell2) {
	switch (cell1->type) {
	case 'S':
	case 'G':
	case ' ':
	case 'M':
		if (add_link(cell1, cell2)) {
			return 1;
		}
	}
	return 0;
}

int add_link(cell_t *cell1, cell_t *cell2) {
cell_t **links_tmp;
	if (cell1->links_n) {
		links_tmp = realloc(cell1->links, sizeof(cell_t *)*(cell1->links_n+1));
		if (links_tmp) {
			cell1->links = links_tmp;
		}
		else {
			fprintf(stderr, "Could not reallocate links\n");
			free(cell1->links);
			return 1;
		}
	}
	else {
		cell1->links = malloc(sizeof(cell_t *));
		if (!cell1->links) {
			fprintf(stderr, "Could not allocate links\n");
			return 1;
		}
	}
	cell1->links[cell1->links_n++] = cell2;
	return 0;
}

int search_path(const char *name, cell_t *first, cell_t *last) {
unsigned long i, j, k;
cell_t *cell;
	first->visited = 1;
	first->from = NULL;
	queue[0] = first;
	queue_size = 1;
	for (i = 0; i < queue_size && queue[i] != last; i++) {
		if (queue[i]->extra) {
			queue[i]->power--;
			queue[i]->extra--;
			add_to_queue(queue[i]);
		}
		else {
			for (j = 0; j < queue[i]->links_n; j++) {
				test_link(queue[i], queue[i]->links[j]);
			}
		}
	}
	printf("\n%s\n\n", name);
	if (i < queue_size) {
		for (cell = last; cell; cell = cell->from) {
			if (cell->type == ' ') {
				cell->type = '*';
			}
		}
		for (i = 0; i < levels_n; i++) {
			for (j = 0; j < length; j++) {
				for (k = 0; k < width; k++) {
					putchar(cells[cell_index(i, j, k)].type);
				}
				puts("");
			}
			if (i < levels_n-1) {
				puts("");
			}
		}
		return 0;
	}
	else {
		printf("Path not found !\n");
		return 1;
	}
}

void add_to_queue(cell_t *cell) {
	queue[queue_size++] = cell;
}

void test_link(cell_t *from, cell_t *link) {
	if (!link->visited && link->full_extra <= from->power) {
		link->power = from->power;
		link->visited = 1;
		link->from = from;
		add_to_queue(link);
	}
}

void reset_cell(cell_t *cell) {
	cell->visited = 0;
	if (cell->type == 'M') {
		cell->extra = cell->full_extra;
	}
}

void free_cells(unsigned long n) {
unsigned long i;
	for (i = 0; i < n; i++) {
		if (cells[i].links_n) {
			free(cells[i].links);
		}
	}
	free(cells);
}
