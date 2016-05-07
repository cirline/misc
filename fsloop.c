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

int file_insert(struct file_node *fnode)
{
	int i;
	struct file_node *new;
	struct table_node tbln;

	if(!fnode)
		return -1;
	memset(&tbln, 0, sizeof(tbln));
	tbln.hash = strhash(fnode->filename, FILE_TABLE_SIZE);

	new = malloc(sizeof(*new));
	if(!new) {
		pr_err("malloc new file node failed: %s\n", strerror(errno));
		return -1;
	}
	new->filename = strdup(fnode->filename);

	tbln.cmp_str = new->filename;
	tbln.p = new;

	return hash_table_insert(tn_table, &tbln, FILE_TABLE_SIZE);
}

int file_remove(struct file_node *fnode)
{
	int i;
	int hash;
	struct file_node *rm;
	struct table_node tnode;

	memset(&tnode, 0, sizeof(tnode));

	tnode.cmp_str = fnode->filename;
	tnode.hash = strhash(fnode->filename, FILE_TABLE_SIZE);

	rm = hash_table_remove(tn_table, &tnode, FILE_TABLE_SIZE);
	if(rm) {
		free(rm->filename);
		free(rm);
	}

	return 0;
}

void pr_table_fn(int i, void *p)
{
	struct file_node *node = p;
	pr_info("%4d, %s\n", i, node->filename);
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
	struct file_node fnode;

	/* scan the dir to build table */
	pr_debug("fdesc table size = %d\n", sizeof(fdesc_table));
	dir_scan(FILESYNC_PATH, file_insert);

	hash_table_print(tn_table, FILE_TABLE_SIZE, pr_table_fn);

	fnode.filename = buffer;
	file_remove(&fnode);

	hash_table_print(tn_table, FILE_TABLE_SIZE, pr_table_fn);

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
