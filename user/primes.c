#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
	int i;
	int intsize = 4;
	int pid;
	int p[2];

	if(argc != 1){
		fprintf(2, "Usage: primes\n");
		exit(1);
	}

	pipe(p);

	for(i = 2; i < 36; i++){
		write(p[1], &i, intsize);
	}

	while((pid = fork()) == 0){
		close(p[1]);
		int first, prime; 
		int fa = p[0];

		if(pipe(p) < 0){
			fprintf(2, "pipe fail\n");
			exit(1);
		}

		if(read(fa, &first, intsize) != 0)fprintf(1, "prime %d\n", first);
		else break;

		while(read(fa, &prime, intsize) != 0){
			if(prime % first != 0)write(p[1], &prime, intsize);
		}
		close(fa);

	}

	if(pid < 0){
		fprintf(2, "fork error!");
		exit(1);
	}
	else if(pid > 0){
		close(p[0]);
		close(p[1]);
		wait(0);
	}

	exit(0);
}
