#ifndef _FIFO_H_
#define _FIFO_H_

struct q_node {
	struct q_node *next;
	struct q_node *prev;
	double a;
	void *element;
};

struct q_node *q_newnode(size_t elsize)
{
	printf("q_node struct: %d\n", sizeof(struct q_node));
	printf("q_node struct + elsize - 1: %d\n", sizeof(struct q_node) + elsize - 1);
	
	return malloc(sizeof(struct q_node) + elsize - 1);
}

struct q_node *n = q_newnode(sizeof(int_fast64_t));
n->element = 999;
int_fast64_t *valp = n->element;
print("%llu\n", *valp);

#endif /* _FIFO_H_*/
