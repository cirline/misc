#define pr_fmt(fmt)	"table: " fmt

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "header.h"

int hash_table_lookup(struct hash_table_desc *ht, struct table_node *node)
{
	int i;
	struct table_node *p;

	if(!ht || !ht->tbl || ht->size <= 0 || !node)
		return -1;

	i = node->hash;
	do {
		p = ht->tbl[i];
		if(p) {
			if(node->cmp_str && !strcmp(p->cmp_str, node->cmp_str))
				return i;
			else if(node->cmp_int == p->cmp_int)
				return i;
		}
		(i >= ht->size - 1) ? i = 0: i++;
	} while (i != node->hash);

	return -1;
}

int hash_table_insert(struct hash_table_desc *ht, struct table_node *node)
{
	int i;
	struct table_node *new;

	if(!ht || !ht->tbl || ht->size <= 0 || !node)
		return -1;

	i = node->hash;
	pr_debug("insert node (hash: %d)\n", node->hash);
	do {
		if(!ht->tbl[i]) {
			new = malloc(sizeof(*new));
			if(!new) {
				pr_err("malloc new node failed: %s\n", strerror(errno));
				return -1;
			}
			if(node->cmp_str)
				new->cmp_str = strdup(node->cmp_str);
			new->cmp_int = node->cmp_int;
			new->hash = node->hash;
			if(ht->insert)
				new->p = ht->insert(node);
			ht->tbl[i] = new;
			return i;
		}
		(i >= ht->size - 1) ? i = 0: i++;
	} while(i != node->hash);
	pr_err("table overflow!\n");

	return -1;
}

int hash_table_remove(struct hash_table_desc *ht, struct table_node *node)
{
	int i;
	struct table_node *rm;

	if(!ht || !ht->tbl || ht->size <= 0 || !node)
		return -1;

	i = hash_table_lookup(ht, node);
	if(i < 0) {
		pr_err("remove node not found.\n");
		return -1;
	}

	rm = ht->tbl[i];

	/* free p */
	if(ht->remove)
		ht->remove(rm->p);
	/* free table node */
	if(rm->cmp_str)
		free(rm->cmp_str);
	free(rm);
	ht->tbl[i] = NULL;

	return 0;
}


int hash_table_print(struct hash_table_desc *ht)
{
	int i;
	int count;
	struct table_node *node;

	if(!ht || !ht->tbl || ht->size <= 0)
		return -1;

	pr_info("***** print table *****\n");
	for(i = 0, count = 0; i < ht->size; i++) {
		node = ht->tbl[i];
		if(node) {
			count++;
			if(ht->print)
				ht->print(i, node->p);
		}
	}
	pr_info("-- --\n");
	pr_info("total %d\n", count);
	pr_info("***** table end *****\n");

	return count;
}



