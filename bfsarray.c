#include <stdio.h>
#include <stdlib.h>
#include <upc_relaxed.h>
#include <upc_collective.h>
#include "upc.h"
#include "new_bfs.h"
#include "upc_task.h"

#define A 16
#define E 144 //change to scan in from file

taskq_t *taskq;
shared long N, step;
shared []struct Edge *shared vertices;
//struct Edge* vertices;
//shared struct EdgeListItem *shared edgelist;
shared []long *shared *shared edgelist;

shared []long *shared maxindex;
shared []long *shared currindex;
shared [] long *shared parentarray;
//int parents[64];
//int *parentarray;
upc_lock_t* list_lock;

//shared int *shared p4;
//Use to build shared linked structures, e.g., a linked list
//shared struct EdgeListItem* shared elist;

// void getTotal()
// {
// 	system("wc coords.txt > line.txt");//fix this later: print the string instead
// 	FILE *file2 = fopen("line.txt", "r");
// 	fscanf(file2, "%ld", E);
// 	fclose(file2);
// 	printf("Total: %ld", E);
// }


/* Retrieve te data of the list of all edges in format <StartVertex, EndVertex> and stores as a one dimensional array of Edge structs.
	This portion can be untimed. */
void create()
{
	long v1, v2, weight;
	N = 0;

	char file_name[10] = "coords.txt";
	long i;
	int r;
	FILE* file;
	file = fopen("coords.txt", "r");
	if (file == NULL) 
	{
		printf("Couldn't open/find coords.txt");
		exit(-1);
	} 

	else 
	{
		printf("HI");

		long g;
		
		for(g = 0; g < E && r != EOF; g++) 
		{
			r = fscanf(file, "(%ld %ld %ld) \n", &v1, &v2, &weight);
			if (v1 > N)
				N = v1;
			if (v2 > N)
				N = v2;
			//process(v1, v2);
			// //printf("before assigning values. g = %ld, curr = %ld, next = %ld, weight = %ld \n", g, curr, next, weight);
			vertices[g].v1 = v1;
			vertices[g].v2 = v2;
			vertices[g].weight = weight;
			// //printf("AFTER assigning values. g = %ld, curr = %ld, next = %ld, weight = %ld \n", g, curr, next, weight);

		}
		N++;
		printf("This is N: %ld", N);
	}
	fclose(file);

}
// void makeVertices()
// {
// 	long i;
// 	for(i = 0; i < N; i++)
// 		vertices[i] = upc_alloc(E*sizeof(struct Edge));
// }

void makeEdgeList()
{
	long i;
	upc_forall(i = 0; i < N; i++; i)
		edgelist[i] = upc_all_alloc(2*A, sizeof(long));
}

void expand(long v)
{
	long c = maxindex[v]; 
	long newmaxindex;
	long *temp = malloc(c*sizeof(long));
	for(long i = 0; i< c; i++)
	{
		temp[i] = edgelist[v][i];
	}
	newmaxindex = 2*maxindex[v];
	edgelist[v] = upc_all_alloc (newmaxindex, sizeof(long));
	for(long i = 0; i < maxindex[v]; i++)
	{
		edgelist[v][i] = temp[i];
	}
	for(long i = maxindex[v]; i < newmaxindex; i++)
	{
		edgelist[v][i] = -1;
	}
	 maxindex[v] = newmaxindex;
}

void makeMaxIndex()
{
	long i;
	upc_forall(i = 0; i < N; i++; i)
	{
		maxindex[i] = 2*A;
	}
}
void makeCurrIndex()
{
	printf("Making currindex: \n");
	long i;
	upc_forall(i = 0; i < N; i++; i)
	{
		currindex[i] = 0;
		printf("index: %ld  value: %ld\n", i, currindex[i]);
	}
}
void makeParents()
{
	long i;
	upc_forall(i = 0; i < N; i++; i)
	{
		parentarray[i] = -1;
	}
}

void process(long v1, long v2)
{
	printf("v1: %ld, v2: %ld\n", v1, v2);
	upc_lock(list_lock);
	long myi1=currindex[v1], myi2=currindex[v2];
	currindex[v1]++;
	currindex[v2]++;
	if (currindex[v2] >= maxindex[v2])
		expand(v2);	
	if(currindex[v1] >= maxindex[v1])
		expand(v1);
	upc_unlock(list_lock);
	printf("myi1: %ld, myi2: %ld", myi1, myi2);
	edgelist[v1][myi1] = v2;
	edgelist[v2][myi2] = v1;
	printf("Value added: (%ld, %ld) and (%ld, %ld) \n", v1, edgelist[v1][myi1], v2, edgelist[v2][myi2]);
		
}

void printList()
{
	long i;
	printf("In printlist %ld\n", N);
	for (i = 0; i < N; i++) 
	{
		int j = 0;
		if (edgelist[i] == NULL) 
		{
			printf("[]\n");
		}
		
		else 
		{
			if(j == currindex[i])
					printf("next edge is null");
			else
				printf("vertex: %ld: ", i);
			printf("[");
			//printf("%ld", edgelist[i].eli->e->next_edge);
			//struct EdgeListItem *next_eli = edgelist[i].eli;
			
			while (j < currindex[i]) {
				
				printf("%ld => ", edgelist[i][j]);
				// if(j < currindex[i])
				// 	next_eli = next_eli->eli;
				// else
				// 	next_eli = NULL;
				j+=1;
			}
			printf("]\n");
		}
	}
}


void traverse(long * root)
{
	
	//upc_barrier;
	printf("In traverse. ");
	if (MYTHREAD==0) 
	{
		taskq_put(taskq, breadth_first_traverse, root, root);//this line causes an error: sigsegv(11)
		printf("Put in the first task \n");
	}
	 do { 
      while(taskq_execute(taskq)) {;}   
    } while ( !taskq_all_isEmpty(taskq) );
   	if (MYTHREAD==0) printf("Completed BFS");
   		taskq_all_free(taskq);
}

void breadth_first_traverse(long* root, long* parent)
{
	if(parentarray[root] != -1)
		return;
	parentarray[root] = *parent;
	long *vchild;
	long i;
	for(i = 0; i < currindex[root]; i++)
	{
		vchild = edgelist[root][i];
		taskq_put(taskq, breadth_first_traverse, vchild, root);
	}
}

/**
 * Main Method. This method creates the data structures and calls other methods. 
 **/
int main()
{
	list_lock = upc_all_lock_alloc(); // allocates memory space for initializing instances of the code
	vertices = upc_all_alloc(E, sizeof(shared [] struct Edge*));	//sizeof(shared [] int *)
	upc_barrier;
	if(MYTHREAD == 0) // initiailzing all the global memory when at first node
	{
		create();
	}
	upc_barrier;	
	printf("N is: %ld, A is: %ld \n", N, A);
	//long x = N*2*A;
	//shared []long *e  = upc_all_alloc(x, sizeof(long));
	edgelist =  upc_all_alloc(N, sizeof(shared [] long*));
	//allocate maxindex and currindex
	maxindex = upc_all_alloc(N, sizeof(shared [] long*));
	currindex = upc_all_alloc(N, sizeof(shared [] long*));
	parentarray = upc_all_alloc(N, sizeof(shared [] long*));
	taskq = taskq_all_alloc(1, breadth_first_traverse, sizeof(shared [] long*), sizeof(shared [] long*));
	//getTotal();	
	upc_barrier;
	makeMaxIndex();
	makeCurrIndex();
	makeEdgeList();
	makeParents();
	upc_barrier;

	long i, v1, v2;
	upc_forall(i = 0; i < E; i++; i) // for statement with affinity field (controls execution of the loop)
	{
		v1 = vertices[i].v1;
		v2 = vertices[i].v2;
		printf("Index: %ld <%ld , %ld> \n", i, v1, v2);
		process(v1, v2);
	}
	upc_barrier;
	printList();
	parentarray[0] = 0;
	traverse(&edgelist[0][0]);
}
