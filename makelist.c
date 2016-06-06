#include <stdio.h>
#include <stdlib.h>

/*node is a vertex of the graph: consists of the weight of its graph edge and a pointer to a neighboring vertex*/
struct node
{
	int val;
	struct node * next;
}

/* first_node represents the "head" of the linkedlist of a path through vertices in the graph. 
  It is also one of the 64 randomly picked "search keys" or starting points in the graph. */
struct first_node
{
	struct node *head;
}

/* graph represents an array of the 64 search keys, each of which are individual first_node structures. 
  Included are the array itself and the amount of nodes in the linkedlist for future reference. */
struct graph
{
	int nodes;
	struct first_node* arr;
}

/* Function for making a graph by creating a graph structure and adding the nodes associated with it. */
graph* makeGraph(int n)
{
	struct graph* g = (struct graph*)malloc(sizeof(struct graph));
	g->nodes = n;
	g->arr = (struct first_node*)malloc(n*sizeof(struct first_node));
	int i;
	for(i=0; i<n; i++)
		g->arr[i].head = NULL;
	return g;
}

/* Function for creating a node.
  Takes an integer value for the weight of the node and returns the head of the list.*/
node* makeNode(int next)
{
	struct node* new_node = (struct node*) malloc(sizeof(struct node));
	new_node->val = next;
	return new_node;
}

/* Function for adding a node to the linked list. 
  Adds the node to the beginning of the list, replacing the current head as the new head.
  Also does the reverse as this is an directed graph by making the current node the new head and pointing to the next node.*/
void addNode(struct graph* g, int curr, int next)
{
	struct node* new_node = makeNode(next);
	new_node->next = g->arr[curr].head;
	g->arr[curr].head = new_node;

	new_node = makeNode(curr);
	new_node->next = g.arr[curr].head;
	g->arr[next].head = new_node;
}


/* Main function for running the code (currently incomplete--still need to add file i/o)*/
main() 
{

	/*n=find total number of nodes searched (graph500=64) (how big array needs to be)
	
	using input tuple list: calculate total nodes (no) and store vals by using the file
	int *xcoor = (int *)malloc(sizeof(*no));
	int *ycoor = (int *)malloc(sizeof(*no));
	
	consider: do i need array to store the x and y values? or should i make them a structure specific variable.
	*/
	
	int n = 4;
	struct graph* g = makeGraph(n);
	
	/* Nodes are added in pairs of node and neighbor--currently no vertex value associated with them. */
	addNode(g, 1, 2);
	addNode(g, 2, 3);
	addNode(g, 2, 4);
	addNode(g, 3, 4);
	addNode(g, 1, 4);
  /* do i need to add nodes in order that they point?
    graph made above:
    
    1. ------ 4.
    |      /  |
    |    /    |
    |  /      |
    2. ------ 3.
    
  */
	printf("done");
  /*print graph output in trees*/
}

