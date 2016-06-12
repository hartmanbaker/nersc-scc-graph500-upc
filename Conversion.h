/**
 * @purpose Creates an adjacency list when given an input file of data (x, y, w).
 * 			List is array of Linked Lists)
 *
 * @author Elizabeth Wang
 */

#include <stdio.h>
#include <stdlib.h>
#include "sglib.h"

int path_counter = 0;

int node_max = 0;

struct Path {
	int start;
	int end;
	int weight;
};

struct Path_List {
	struct Path path;
	struct Path_List *next_path_list_ptr;
};

/**
 * Organizes list and reads the input file.
 */
struct Path_List* readFile() {
	struct Path_List *aList;
	static struct Path_List *head;
	head = NULL;
	int r;
	int start, end, weight = 0;

	char file_name[1000];
	int i;

	printf("Please enter the name of a file:");
	scanf("%s", file_name, &i);

	FILE* file;
	file=fopen(file_name, "r");
	if (file == NULL) {
		printf("Failed to open file 'Coords.txt'");
		exit(-1);
	} else {
		r = fscanf(file, "(%d %d %d)\n", &start, &end, &weight);
		while (r != EOF) {
			path_counter++;
			if (start > node_max) {
				node_max = start;
			}
			if (end > node_max) {
				node_max = end;
			}
			struct Path path;
			path.start = start;
			path.end = end;
			path.weight = weight;

			aList = malloc(sizeof(struct Path_List));
			aList->path = path;

			SGLIB_LIST_ADD(struct Path_List, head, aList, next_path_list_ptr);

			printf("(%d %d %d)\n", start, end, weight);

			r = fscanf(file, "(%d %d %d)\n", &start, &end, &weight);
		}
	}
	fclose(file);

	return head;
}

struct ilist {
	int i;
	struct ilist *next_ptr;
};

/**
 * Converts given data and data structures into adjacency list
 */
void convert(struct Path_List* head, struct ilist* adjacency_list[], int total_num_of_nodes) {
	struct ilist* li;

	struct Path_List* ptr = head;
	while (ptr) {
		struct Path p = ptr->path;
		printf("In main, (%d %d %d)\n", p.start, p.end, p.weight);

		if (adjacency_list[p.start] != NULL) {
			struct ilist* al_head = adjacency_list[p.start];
			li = malloc(sizeof(struct ilist));
			li->i = p.end;
			li->next_ptr = NULL;
			SGLIB_LIST_ADD(struct ilist, al_head, li, next_ptr); // macro - can process ilist struct and pathlist
			adjacency_list[p.start] = al_head;
		} else {
			li = malloc(sizeof(struct ilist));
			li->i = p.end;
			li->next_ptr = NULL;
			adjacency_list[p.start] = li;
		}

		if (adjacency_list[p.end]) {
			struct ilist* al_head = adjacency_list[p.end];
			li = malloc(sizeof(struct ilist));
			li->i = p.start;
			li->next_ptr = NULL;
			SGLIB_LIST_ADD(struct ilist, al_head, li, next_ptr);
			adjacency_list[p.end] = al_head;
		} else {
			li = malloc(sizeof(struct ilist));
			li->i = p.start;
			li->next_ptr = NULL;
			adjacency_list[p.end] = li;
		}

		ptr = ptr->next_path_list_ptr;
	}
}


