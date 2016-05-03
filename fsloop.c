#define pr_fmt(fmt)	"fsloop: " fmt

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#include <sys/stat.h>

#include "../utils/log.h"
#include "../utils/ustring.h"
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

			pr_debug("%s\n", dir->d_name);
			pr_s2hex("%s", dir->d_name);
		}

		if(!dir)
			node = put_dir_node(node);
		else
			node = new_dir_node(node, cur);
	}


	return count;
}

int file_insert(struct file_desc *desc)
{
	if(!desc)
		return -1;
	desc->hash = strhash(desc->filename, FILE_TABLE_SIZE);

	i = desc->hash;
	do {
		if(!fdesc_table[i]) {
			fdesc_table[i] = desc;
			return i;
		}
		(i >= FILE_TABLE_SIZE - 1) ? i = 0: i++;
	} while(i != desc->hash);
	pr_err("table overflow!\n");

	return -1;
}

int fsloop(void)
{
	pr_info("fdesc table size = %d\n", sizeof(fdesc_table));
	memset(fdesc_table, 0, sizeof(fdesc_table));
//	dir_scan(FILESYNC_PATH);

	return 0;
}
