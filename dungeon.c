#include <stdio.h>
#include <stdlib.h>

typedef struct cell_s cell_t;

struct block_s {
	int type;
	cell_t *cell;
};
typedef struct block_s block_t;

struct cell_s {
	block_t *block;
	unsigned long power;
	int visited;
	unsigned long cost;
	unsigned long cost_tmp;
	unsigned long links_n;
	cell_t **links;
	cell_t *from;
};

int set_block(block_t *, int);
int set_cell(cell_t *, unsigned long, unsigned long, unsigned long, block_t *);
int z_links(block_t *, cell_t *);
int yx_links(block_t *, cell_t *);
int yx_link(block_t *, cell_t *);
int add_link(cell_t *, cell_t *);
int search_path(const char *, cell_t *, cell_t *);
void add_to_queue(cell_t *);
void test_link(cell_t *, cell_t *);
void reset_cell(cell_t *);
void free_cells(void);

unsigned long height, length, width, power, surface, cells_n, queue_size;
block_t *blocks;
cell_t *cells, *start, *goal, **queue;

int main(void) {
unsigned long blocks_n, b, c, i, j, k;
cell_t *cell;
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
	scanf("%lu", &power);
	fgetc(stdin);
	surface = width*length;
	blocks_n = surface*height;
	blocks = malloc(sizeof(block_t)*blocks_n);
	if (!blocks) {
		fprintf(stderr, "Could not allocate blocks\n");
		return EXIT_FAILURE;
	}
	cells_n = 0;
	b = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < length; j++) {
			for (k = 0; k < width; k++, b++) {
				if (set_block(&blocks[b], fgetc(stdin))) {
					free(blocks);
					return EXIT_FAILURE;
				}
			}
			fgetc(stdin);
		}
		if (i < height-1) {
			fgetc(stdin);
		}
	}
	cells = malloc(sizeof(cell_t)*cells_n);
	if (!cells) {
		fprintf(stderr, "Could not allocate cells\n");
		free(blocks);
		return EXIT_FAILURE;
	}
	start = NULL;
	goal = NULL;
	queue_size = 0;
	b = 0;
	c = 0;
	for (i = 0; i < height; i++) {
		for (j = 0; j < length; j++) {
			for (k = 0; k < width; k++, b++) {
				if (blocks[b].type != '#') {
					if (set_cell(&cells[c], i, j, k, &blocks[b])) {
						free_cells();
						free(blocks);
						return EXIT_FAILURE;
					}
					c++;
				}
			}
		}
	}
	if (!start) {
		fprintf(stderr, "Start not set\n");
		free_cells();
		free(blocks);
		return EXIT_FAILURE;
	}
	if (!goal) {
		fprintf(stderr, "Goal not set\n");
		free_cells();
		free(blocks);
		return EXIT_FAILURE;
	}
	queue = malloc(sizeof(cell_t *)*queue_size);
	if (!queue) {
		fprintf(stderr, "Could not allocate queue\n");
		free_cells();
		free(blocks);
		return EXIT_FAILURE;
	}
	if (!search_path("THE QUEST", start, goal)) {
		for (cell = goal; cell; cell = cell->from) {
			switch (cell->block->type) {
			case 'M':
				cell->block->type = '.';
				cell->cost = 0;
				cell->cost_tmp = 0;
				break;
			case 'D':
				cell->block->type = 'd';
				break;
			case 'U':
				cell->block->type = 'u';
				break;
			case '*':
				cell->block->type = '.';
			}
		}
		for (i = 0; i < queue_size; i++) {
			reset_cell(queue[i]);
		}
		search_path("THE WAY BACK", goal, start);
	}
	free(queue);
	free_cells();
	free(blocks);
	return EXIT_SUCCESS;
}

int set_block(block_t *block, int type) {
	switch (type) {
	case 'S':
	case 'G':
	case 'm':
	case 'd':
	case 'u':
	case '.':
		cells_n++;
		break;
	case '#':
		block->cell = NULL;
		break;
	default:
		fprintf(stderr, "Invalid type\n");
		return 1;
	}
	block->type = type;
	return 0;
}

int set_cell(cell_t *cell, unsigned long z, unsigned long y, unsigned long x, block_t *block) {
	block->cell = cell;
	cell->block = block;
	if (block->type == 'S') {
		if (start) {
			fprintf(stderr, "Start already set\n");
			return 1;
		}
		else {
			start = cell;
			cell->power = power;
		}
	}
	else if (block->type == 'G') {
		if (goal) {
			fprintf(stderr, "Goal already set\n");
			return 1;
		}
		else {
			goal = cell;
		}
	}
	cell->visited = 0;
	cell->cost = block->type == 'm' ? z+1:0;
	cell->cost_tmp = cell->cost;
	cell->links_n = 0;
	if (z && z_links(block-surface, cell)) {
		return 1;
	}
	if (y && yx_links(block-width, cell)) {
		return 1;
	}
	if (x && yx_links(block-1, cell)) {
		return 1;
	}
	queue_size += 1+cell->cost;
	return 0;
}

int z_links(block_t *remote, cell_t *cell) {
	if (remote->type == 'd' && add_link(remote->cell, cell)) {
		return 1;
	}
	if (remote->cell && cell->block->type == 'u' && add_link(cell, remote->cell)) {
		return 1;
	}
	return 0;
}

int yx_links(block_t *remote, cell_t *cell) {
	if (yx_link(remote, cell)) {
		return 1;
	}
	if (remote->cell && yx_link(cell->block, remote->cell)) {
		return 1;
	}
	return 0;
}

int yx_link(block_t *block1, cell_t *cell2) {
	switch (block1->type) {
	case 'S':
	case 'G':
	case 'm':
	case '.':
		if (add_link(block1->cell, cell2)) {
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
unsigned long b, i, j, k;
cell_t *cell;
	first->visited = 1;
	first->from = NULL;
	queue[0] = first;
	queue_size = 1;
	for (i = 0; i < queue_size && queue[i] != last; i++) {
		if (queue[i]->cost_tmp) {
			queue[i]->power--;
			queue[i]->cost_tmp--;
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
			switch (cell->block->type) {
			case 'm':
				cell->block->type = 'M';
				break;
			case 'd':
				cell->block->type = 'D';
				break;
			case 'u':
				cell->block->type = 'U';
				break;
			case '.':
				cell->block->type = '*';
			}
		}
		b = 0;
		for (i = 0; i < height; i++) {
			for (j = 0; j < length; j++) {
				for (k = 0; k < width; k++, b++) {
					putchar(blocks[b].type);
				}
				puts("");
			}
			if (i < height-1) {
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
	if (!link->visited && link->cost <= from->power) {
		link->power = from->power;
		link->visited = 1;
		link->from = from;
		add_to_queue(link);
	}
}

void reset_cell(cell_t *cell) {
	cell->visited = 0;
	if (cell->block->type == 'm') {
		cell->cost_tmp = cell->cost;
	}
}

void free_cells(void) {
unsigned long i;
	for (i = 0; i < cells_n; i++) {
		if (cells[i].links_n) {
			free(cells[i].links);
		}
	}
	free(cells);
}
