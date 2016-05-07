#define pr_fmt(fmt)	"table: " fmt

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "header.h"

int hash_table_lookup(struct table_node *head[], struct table_node *node, int table_size)
{
	int i;
	struct table_node *p;

	if(!head || !node || table_size <= 0)
		return -1;

	i = node->hash;

	do {
		p = head[i];
		if(p) {
			if(node->cmp_str && !strcmp(p->cmp_str, node->cmp_str))
				return i;
			else if(node->cmp_int == p->cmp_int)
				return i;
		}
		(i >= table_size - 1) ? i = 0: i++;
	} while (i != node->hash);

	return -1;
}

int hash_table_insert(struct table_node *head[], struct table_node *node, int table_size)
{
	int i;
	unsigned int hash;
	struct table_node *new;

	if(!head || !node || table_size <= 0)
		return -1;

	i = node->hash;
	pr_debug("insert node (hash: %d)\n", node->hash);
	do {
		if(!head[i]) {
			new = malloc(sizeof(*new));
			if(!new) {
				pr_err("malloc new node failed: %s\n", strerror(errno));
				return -1;
			}
			if(node->cmp_str)
				new->cmp_str = strdup(node->cmp_str);
			new->cmp_int = node->cmp_int;
			new->hash = node->hash;
			new->p = node->p;
			head[i] = new;
			return i;
		}
		(i >= table_size - 1) ? i = 0: i++;
	} while(i != node->hash);
	pr_err("table overflow!\n");

	return -1;
}

void * hash_table_remove(struct table_node *head[], struct table_node *node, int table_size)
{
	int i;
	int hash;
	struct table_node *rm;
	void *p;

	if(!head || !node || table_size <= 0)
		return NULL;

	i = hash_table_lookup(head, node, table_size);
	if(i < 0) {
		pr_err("remove node not found.\n");
		return NULL;
	}

	rm = head[i];
	p = rm->p;
	if(rm->cmp_str)
		free(rm->cmp_str);
	free(rm);
	head[i] = NULL;

	return p;
}


int hash_table_print(struct table_node *head[], int table_size, tbl_custom_pr_t pr_fn)
{
	int i;
	int count;
	struct table_node *node;

	if(!head || table_size <= 0)
		return -1;

	pr_info("***** print table *****\n");
	for(i = 0, count = 0; i < table_size; i++) {
		node = head[i];
		if(node) {
			count++;
			if(pr_fn)
				pr_fn(i, node->p);
		}
	}
	pr_info("-- --\n");
	pr_info("total %d\n", count);
	pr_info("***** table end *****\n");

	return count;
}



