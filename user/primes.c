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
	close(p[0]);

	int num[36];
	for(i = 1; i < 36; i++){
		num[i] = i;
		write(p[1], &num[i], intsize);
	}

	int cnt = 0;
	while((pid = fork()) == 0){
		int first, prime; 
		int fa = p[1];
		pipe(p);
		close(p[0]);

		if(read(fa, &first, intsize) != 0)printf("prime %d\n", first);
		else break;
		printf("%d %d\n", ++cnt, first);

		while(read(fa, &prime, intsize) != 0){
			if(prime % first != 0)write(p[1], &prime, intsize);
		}

		close(fa);
	}

	if(pid < 0){
		fprintf(2, "fork error!");
		exit(1);
	}
	else if(pid > 0)wait(0);

	exit(0);
}
