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

static struct file_desc * fdesc_table[FILE_TABLE_SIZE];

/**
 * test file type
 * <1: error
 * =0: dir
 * >0: file
 */
int file_type(const char *path)
{
	struct stat sbuf;

	if(stat(path, &sbuf)) {
		pr_err("stat %s failed: %s\n", strerror(errno));
		return -1;
	}

	if(S_ISDIR(sbuf.st_mode)) {
		return 0;
	} else {
		return 1;
	}


}

struct dir_node * new_dir_node(struct dir_node *parent, const char * path)
{
	struct dir_node *node;

	node = malloc(sizeof(struct dir_node));
	if(!node) {
		pr_err("malloc node failed: %s\n", strerror(errno));
		goto err;
	}
	node->path = strdup(path);
	if(!node->path) {
		pr_err("dup path failed: %s\n", strerror(errno));
		goto err_dup_path;
	}

	node->prev = parent;
	node->d = opendir(path);
	if(!node->d) {
		pr_err("open dir %s failed: %s\n", path, strerror(errno));
		free(node);
		return NULL;
	}

	return node;

err_dup_path:
	free(node);
err:
	return NULL;
}

struct dir_node * put_dir_node(struct dir_node *node)
{
	struct dir_node *p;

	if(!node)
		return NULL;

	closedir(node->d);
	p = node->prev;
	free(node);

	return p;
}

int dir_scan()
{
	DIR *d;
	struct dirent *dir;
	int count = 0;
	struct dir_node *node;
	char cur[1024];
	int type;
	struct file_desc desc;
	int rc;

	node = new_dir_node(NULL, FILESYNC_PATH);

	while(node) {
		while(dir = readdir(node->d)) {
			if(*dir->d_name  == '.')
				continue;

			sprintf(cur, "%s/%s", node->path, dir->d_name);
			type = file_type(cur);

			if(type == 0) {
				pr_debug("dir: %s\n", cur);
				break;
			}
			desc.filename = dir->d_name;
			rc = file_insert(&desc);
		}

		if(!dir)
			node = put_dir_node(node);
		else
			node = new_dir_node(node, cur);
	}


	return count;
}

int file_lookup(char *filename)
{
	unsigned int hash;
	int i;
	struct file_desc *desc;

	hash = strhash(filename, FILE_TABLE_SIZE);

	i = hash;

	do {
		desc = fdesc_table[i];
		if(desc) {
			if(strcmp(filename, desc->filename) == 0)
				return i;
		}
		(i >= FILE_TABLE_SIZE - 1) ? i = 0: i++;
	} while (i != hash);

	return -1;
}

int file_insert(struct file_desc *desc)
{
	int i;
	unsigned int hash;
	struct file_desc *new;

	if(!desc)
		return -1;
	hash = strhash(desc->filename, FILE_TABLE_SIZE);

	i = hash;
	pr_debug("insert file, %s (hash: %d)\n", desc->filename, hash);
	do {
		if(!fdesc_table[i]) {
			new = malloc(sizeof(struct file_desc));
			if(!new) {
				pr_err("malloc new file_desc failed: %s\n", strerror(errno));
				return -1;
			}
			new->filename = strdup(desc->filename);
			new->hash = hash;
			fdesc_table[i] = new;
			return i;
		}
		(i >= FILE_TABLE_SIZE - 1) ? i = 0: i++;
	} while(i != desc->hash);
	pr_err("table overflow!\n");

	return -1;
}

int file_remove(struct file_desc *desc)
{
	int i;
	int hash;
	struct file_desc *rm;

	i = file_lookup(desc->filename);
	if(i < 0) {
		pr_err("%s not found.\n");
		return -1;
	}

	rm = fdesc_table[i];
	free(rm->filename);
	free(rm);
	fdesc_table[i] = NULL;

	return 0;
}

int pr_table(void)
{
	int i;
	int count;
	struct file_desc *desc;

	pr_info("***** print table *****\n");
	for(i = 0, count = 0; i < FILE_TABLE_SIZE; i++) {
		desc = fdesc_table[i];
		if(desc) {
			count++;
			pr_info("%4d, %s\n", i, desc->filename);
		}
	}
	pr_info("-- --\n");
	pr_info("total %d\n", count);
	pr_info("***** table end *****\n");

	return count;
}

int fsloop(void)
{
	int rc;
	struct file_desc *desc;
	int fd_sock;
	int fd_cli;
	struct sockaddr_in cli_addr;
	int len;
	char buffer[1024] = "hello world.";

	/* scan the dir to build table */
	pr_debug("fdesc table size = %d\n", sizeof(fdesc_table));
	memset(fdesc_table, 0, sizeof(fdesc_table));
	dir_scan(FILESYNC_PATH);
	pr_table();

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

	return 0;
}
