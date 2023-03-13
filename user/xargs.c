#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int i;
	int pid;
	char buf[512];
	char* p = buf;
	char* full_argvs[32];
	if(argc < 2){
		fprintf(2, "usage: xargs commands\n");
		exit(1);
	}

	for(i = 1; i < argc; i++){
		full_argvs[i - 1] = argv[i];
	}
	full_argvs[argc] = 0;

	while(read(0, p, 1) != 0){
		if(*p != '\n'){
			p++;
			continue;
		}
		*p = 0;

		full_argvs[argc - 1] = buf;
		pid = fork();
		if(pid < 0){
			fprintf(2, "fork error\n");
			exit(1);
		}
		else if(pid == 0){
			exec(full_argvs[0], full_argvs);
			fprintf(2, "exec failed\n");
			exit(1);
		}
		else{
			wait(0);
			p = buf;
		}
	}
	exit(0);
}
