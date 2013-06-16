#ifndef __LINK_2_H__
#define __LINK_2_H__

#include "stdlib.h"
#include "assert.h"

#ifdef __cplusplus
extern "C"
{
#endif 
	 
typedef struct _node2 {
	struct _node2 *next;
	struct _node2 *pre;
} node2;

typedef struct _list_header {
	node2 head;
	unsigned int count;
} list_header;

void init_list(list_header *h);
void add_2_head(list_header *h, node2 *n);
void add_2_tail(list_header *h, node2 *n);
node2 *delete_node(list_header *h, node2 *n);
node2 *head_node(const list_header *list_head);
node2 *tail_node(const list_header *list_head);
node2 *next_node(const list_header *list_head, node2* n);
node2 *pre_node(const list_header *list_head, node2 *n);
#ifdef __cplusplus
}
#endif

#endif