/*
 * Conversion.c
 *
 *  Created on: Jun 5, 2016
 *      Author: elizabeth wang
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

struct Path_List{
	struct Path path;
	struct Path_List *next_path_list_ptr;
};

/*
int main(void){
	printf("Start reading the input");
}
*/

struct Path_List* readFile(){
	struct Path_List *aList;
	static struct Path_List *head;
	head = NULL;
	int r;
	int start, end, weight = 0;
	FILE* file;
	file = fopen("Coords.txt", "r");
	if (file == NULL){
		printf("Failed to open file 'Coords.txt'");
		exit(-1);
	}else{
		r = fscanf(file, "(%d %d %d)\n", &start, &end, &weight);
		while(r != EOF){
			path_counter++;
			if(start > node_max){
				node_max = start;
			}
			if(end > node_max){
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

#define ILIST_COMPARATOR(e1, e2) (e1->i - e2->i)

int main(int argc, char **argv) {
	struct Path_List* head = readFile();

	// nodes range from 0 to node_max
	int total_num_of_nodes = node_max + 1;
	struct ilist* adjancency_list[total_num_of_nodes];
	  int i,a;
	for(i = 0; i < total_num_of_nodes; i++){
		adjancency_list[i] = NULL;
	}

	struct ilist* li;

	struct Path_List* ptr = head;
	while(ptr){
		struct Path p = ptr->path;
		printf("In main, (%d %d %d)\n", p.start, p.end, p.weight);

		if(adjancency_list[p.start] != NULL){
			struct ilist* al_head = adjancency_list[p.start];
			li = malloc(sizeof(struct ilist));
			li->i = p.end;
			li->next_ptr = NULL;
			SGLIB_LIST_ADD(struct ilist, al_head, li, next_ptr);
			adjancency_list[p.start] = al_head;
		}else{
			li = malloc(sizeof(struct ilist));
			li->i = p.end;
			li->next_ptr = NULL;
			adjancency_list[p.start] = li;
		}

		if(adjancency_list[p.end]){
			struct ilist* al_head = adjancency_list[p.end];
			li = malloc(sizeof(struct ilist));
			li->i = p.start;
			li->next_ptr = NULL;
			SGLIB_LIST_ADD(struct ilist, al_head, li, next_ptr);
			adjancency_list[p.end] = al_head;
		}else{
			li = malloc(sizeof(struct ilist));
			li->i = p.start;
			li->next_ptr = NULL;
			adjancency_list[p.end] = li;
		}

		ptr = ptr->next_path_list_ptr;
	}
	printf("In total, there are %d paths.\n", path_counter);
	printf("In total, there are %d nodes.\n", total_num_of_nodes);

	for(i = 0; i < total_num_of_nodes; i ++){
		printf("node %d: ", i);
		if(adjancency_list[i] == NULL) {
			printf("[]\n");
		}else{
			printf("[");
			struct ilist* al_head = adjancency_list[i];
			while(al_head != NULL){
				printf("%d => ", al_head->i);
				al_head = al_head->next_ptr;
			}
			printf("]\n");
		}
	}

  return(0);
}


