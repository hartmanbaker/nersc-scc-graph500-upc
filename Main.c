/*
 * main.c
 *
 *  Created on: Jun 11, 2016
 *      Author: elizabeth
 */
#include "conversion.h"

int main(int argc, char **argv) {
	struct Path_List* head = readFile();
	int total_num_of_nodes = node_max+1;

	struct ilist** adjacency_list = malloc(total_num_of_nodes * sizeof(struct ilist*));
	int i;

	convert(head, adjacency_list, total_num_of_nodes);

	printf("In total, there are %d paths.\n", path_counter);
	printf("In total, there are %d nodes.\n", total_num_of_nodes);

	for (i = 0; i < total_num_of_nodes; i++) {
		printf("node %d: ", i);
		if (adjacency_list[i] == NULL) {
			printf("[]\n");
		} else {
			printf("[");
			struct ilist
			*al_head = adjacency_list[i];
			while (al_head != NULL) {
				printf("%d => ", al_head->i);
				al_head = al_head->next_ptr;
			}
			printf("]\n");
		}
	}

	return (0);
}

