#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int pid;
	char* message = "message";
	int messagesize = 8;
	int p[2];
	char buf[16];

	if(pipe(p) < 0){
		fprintf(2, "pipe failed\n");
		exit(1);
	}

	pid = fork();

	if(pid == 0){
		read(p[0], buf, messagesize);
		write(p[1], message, messagesize);

		// fprintf(1, "%s: son\n", buf);
		fprintf(1, "%d: received ping\n", getpid());
	} else{
		write(p[1], message, messagesize);
		wait(0);
		read(p[0], buf, messagesize);

		// fprintf(1, "%s: father\n", buf);
		fprintf(1, "%d: received pong\n", getpid());
	}

	exit(0);
}
