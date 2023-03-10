#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int pid;

	pid = fork();

	if(pid == 0){
		fprintf(1, "%d: received ping\n", getpid());
	} else{
		file = pipe(
		fprintf(1, "%d: received pong\n", getpid());
	}

	exit(0);
}
