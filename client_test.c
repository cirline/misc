#include <sys/socket.h>
#include <netinet/in.h>

#include "../utils/header.h"
#include "filesync.h"

int main(void)
{
	int fd;
	char buffer[1024];
	int rc;

	fd = new_client_socket("127.0.0.1", NET_PORT);
	if(fd < 0) {
		pr_err("new client socket failed.\n");
		return -1;
	}

	rc = read(fd, buffer, 1024);

	pr_info("recv: %s\n", buffer);


	return 0;
}
