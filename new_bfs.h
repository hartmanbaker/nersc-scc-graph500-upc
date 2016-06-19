#ifndef BFSNEW
#define BFSNEW



struct Edge
{
	long v1;
	long v2;
	long weight;
};
struct EdgeListItem
{
	struct Edge *e;
	struct EdgeListItem *eli;
};
void addEdgeListItem(struct Edge* e_add, long *index);
void create();
void makeEdgeList();
void makeGraph();
void breadth_first_traverse(long *root, long *parent);
void traverse(long root);
void makeParentArr();

void printList();

#endif 
