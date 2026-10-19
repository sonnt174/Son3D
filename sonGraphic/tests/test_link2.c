#include <stdio.h>
#include "link2.h"

static int tap_no;
static int tap_fails;
#define ok(c, name) do { int r_ = (c); \
	printf("%s %d - %s\n", r_ ? "ok" : "not ok", ++tap_no, name); tap_fails += !r_; } while (0)

int main(void)
{
	list_header h;
	node2 a, b, c;

	printf("1..15\n");
	init_list(&h);
	ok(h.count == 0, "init_list sets count to 0");
	ok(head_node(&h) == &h.head && tail_node(&h) == &h.head, "empty list reports its own header as head and tail");
	ok(delete_node(&h, NULL) == NULL, "delete_node of NULL is NULL");
	ok(delete_node(&h, (node2 *)&h) == NULL, "delete_node of the header is NULL");
	ok(next_node(&h, NULL) == NULL && pre_node(NULL, &a) == NULL, "next_node and pre_node reject NULL arguments");
	add_2_tail(&h, &a);
	ok(h.count == 1, "add_2_tail bumps count to 1");
	ok(head_node(&h) == &a && tail_node(&h) == &a, "a single node is both head and tail");
	ok(next_node(&h, &a) == &h.head && pre_node(&h, &a) == &h.head, "the single node links back to the header both ways");
	add_2_tail(&h, &b);
	add_2_head(&h, &c);
	ok(h.count == 3, "count counts every added node");
	ok(head_node(&h) == &c && tail_node(&h) == &b, "add_2_head prepends while add_2_tail appends");
	ok(next_node(&h, &c) == &a && pre_node(&h, &b) == &a, "the middle node is reachable from both ends");
	ok(delete_node(&h, &a) == &a, "delete_node returns the unlinked node");
	ok(h.count == 2 && a.next == NULL && a.pre == NULL, "delete_node nulls the links of the node and decrements count");
	ok(next_node(&h, &c) == &b && pre_node(&h, &b) == &c, "deleting the middle node joins its neighbours");
	delete_node(&h, &c);
	delete_node(&h, &b);
	ok(h.count == 0 && head_node(&h) == &h.head && tail_node(&h) == &h.head, "deleting every node restores the empty list");
	return tap_fails == 0 ? 0 : 1;
}
