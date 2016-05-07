//#define DEBUG

#define pr_fmt(fmt)	"fsloop: " fmt

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//#include "../utils/log.h"
//#include "../utils/ustring.h"
#include "../utils/header.h"
#include "filesync.h"

#define FILE_TABLE_SIZE		1024

static struct table_node * tn_table[FILE_TABLE_SIZE];

static struct hash_table_desc htdesc;

void pr_table_fn(int i, void *p)
{
	struct file_node *node = p;
	pr_info("%4d, %s\n", i, node->filename);
}

int build_node(struct table_node *tnode, struct file_node *fnode, char *name)
{
	int hash;

	fnode->filename = name;

	memset(tnode, 0, sizeof(*tnode));
	tnode->p = fnode;
	tnode->cmp_str = name;
	tnode->hash = strhash(name, FILE_TABLE_SIZE);

	return 0;
}

int do_each_file(struct file_node *node)
{
	struct table_node tnode;
	struct file_node fnode;

	build_node(&tnode, &fnode, node->filename);
	hash_table_insert(&htdesc, &tnode);

	return 0;
}

int fsloop(void)
{
	int rc;
	struct file_node *desc;
	int fd_sock;
	int fd_cli;
	struct sockaddr_in cli_addr;
	int len;
	char buffer[1024] = "xiaopinguo.mp3";
	struct table_node tnode;
	struct file_node fnode;

	htdesc.tbl = tn_table;
	htdesc.size = FILE_TABLE_SIZE;

	htdesc.insert = (void *(*)(void *))new_file_node;
	htdesc.remove = (void (*)(void *))free_file_node;
	htdesc.print = pr_table_fn;

	/* scan the dir to build table */
	dir_scan(FILESYNC_PATH, do_each_file);

	hash_table_print(&htdesc);

	build_node(&tnode, &fnode, buffer);
	hash_table_remove(&htdesc, &tnode);

	hash_table_print(&htdesc);
#if 0
	fd_sock = new_server_socket(NET_PORT, NET_QUEUE);
	if(fd_sock < 0) {
		pr_err("new socket failed.\n");
		return -1;
	}

	fd_cli = accept(fd_sock, (struct sockaddr *)&cli_addr, &len);
	if(fd_cli < 0) {
		pr_err("accept failed: %s\n", strerror(errno));
		return -1;
	}
	write(fd_cli, buffer, 1024);
#endif
	return 0;
}
