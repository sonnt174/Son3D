#include "link2.h"

void init_list(list_header *h)
{
	//assert(h);
	h->head.next = h->head.pre = &h->head;
	h->count = 0;
}

void add_2_head(list_header *h, node2 *n)
{
	//assert(n);
	n->next = h->head.next;
	h->head.next->pre = n;
	n->pre = &h->head;
	h->head.next = n;

	h->count++;
}

void add_2_tail(list_header *h, node2 *n)
{
	//assert(n);

	n->pre = h->head.pre;
	h->head.pre->next = n;
	n->next = &h->head;
	h->head.pre = n;

	h->count++;
}

node2 *delete_node(list_header *h, node2 *n)
{
	node2 *retc;

	if (!h || !n || (h == (list_header*)n)) {
		retc = NULL;
		goto exit;
	}

	n->next->pre = n->pre;
	n->pre->next = n->next;
	n->next = n->pre = NULL;
	h->count--;
	retc = n;

exit:
	return retc;
}

node2 *head_node(const list_header *list_head)
{
	node2 *retc;
	if (!list_head ||
		NULL == list_head->head.next) {
			retc = NULL;
			goto exit;
	}

	retc = list_head->head.next;

exit:
	return retc;
}

node2 *tail_node(const list_header *list_head)
{
	node2 *retc;
	if (!list_head ||
		NULL == list_head->head.pre) {
			retc = NULL;
			goto exit;
	}

	retc = list_head->head.pre;

exit:
	return retc;
}

node2 *next_node(const list_header *list_head, node2* n)
{
	node2 *retc;

	if (!list_head || !n) {
		retc = NULL;
		goto exit;
	}

	retc = n->next;

exit:
	return retc;
}

node2 *pre_node(const list_header *list_head, node2 *n)
{
	node2 *retc;

	if (!list_head || !n) {
		retc = NULL;
		goto exit;
	}

	retc = n->pre;

exit:
	return retc;
}