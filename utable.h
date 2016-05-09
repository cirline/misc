#ifndef __UTILS_TABLE_H__
#define __UTILS_TABLE_H__

struct table_node {
	char *cmp_str;
	int cmp_int;
	int hash;

	void *p;
};

struct hash_table_desc {
	struct table_node **tbl;
	int size;

	void * (*insert)(void *node);
	void (*remove)(void *p);
	void (*foreach)(int index, void *p);
	void (*print)(int index, void *p);
};

typedef void (*tbl_custom_pr_t)(int index, void *p);

extern int hash_table_lookup(struct hash_table_desc *ht, struct table_node *node);
extern int hash_table_insert(struct hash_table_desc *ht, struct table_node *node);
extern int hash_table_remove(struct hash_table_desc *ht, struct table_node *node);
extern int hash_table_print(struct hash_table_desc *ht);

#endif
