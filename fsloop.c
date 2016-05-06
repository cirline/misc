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

static struct file_node * fdesc_table[FILE_TABLE_SIZE];

int file_lookup(char *filename)
{
	unsigned int hash;
	int i;
	struct file_node *desc;

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

int file_insert(struct file_node *desc)
{
	int i;
	unsigned int hash;
	struct file_node *new;

	if(!desc)
		return -1;
	hash = strhash(desc->filename, FILE_TABLE_SIZE);

	i = hash;
	pr_debug("insert file, %s (hash: %d)\n", desc->filename, hash);
	do {
		if(!fdesc_table[i]) {
			new = malloc(sizeof(struct file_node));
			if(!new) {
				pr_err("malloc new file_node failed: %s\n", strerror(errno));
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

int file_remove(struct file_node *desc)
{
	int i;
	int hash;
	struct file_node *rm;

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
	struct file_node *desc;

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
	struct file_node *desc;
	int fd_sock;
	int fd_cli;
	struct sockaddr_in cli_addr;
	int len;
	char buffer[1024] = "hello world.";

	/* scan the dir to build table */
	pr_debug("fdesc table size = %d\n", sizeof(fdesc_table));
	memset(fdesc_table, 0, sizeof(fdesc_table));
	dir_scan(FILESYNC_PATH, file_insert);
	pr_table();
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
