#ifndef __UTILS_TABLE_H__
#define __UTILS_TABLE_H__

struct table_node {
	char *cmp_str;
	int cmp_int;

	int hash;

	void *p;
};

void * (*tn_custom_insert_t)(void);
void (*tn_custom_remove_)(void);
typedef void (*tbl_custom_pr_t)(int index, void *p);

extern int hash_table_lookup(struct table_node *head[], struct table_node *node, int table_size);
extern int hash_table_insert(struct table_node *head[], struct table_node *node, int table_size);
extern void * hash_table_remove(struct table_node *head[], struct table_node *node, int table_size);
extern int hash_table_print(struct table_node *head[], int table_size, tbl_custom_pr_t pr_fn);

#endif
