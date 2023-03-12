#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
	buf[strlen(p)] = 0;
  return buf;
}

void
find(char *path, char *name)
{
	char buf[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

	if(st.type != T_DIR){
    fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
		printf("find: path too long\n");
		return;
	}
	strcpy(buf, path);
	p = buf+strlen(buf);
	*p++ = '/';

	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if(de.inum == 0)
			continue;
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buf, &st) < 0){
			printf("ls: cannot stat %s\n", buf);
			continue;
		}

		if(st.type == T_FILE){
			if(strcmp(fmtname(buf), name) == 0){
				printf("%s\n", buf);
			}
		}
		else if(st.type == T_DIR){
			char* dirname = fmtname(buf);

			if(strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0){
				continue;
			}
			if(strcmp(dirname, name) == 0){
				printf("%s/\n", buf);
			}
			find(buf, name);
		}
	}
	close(fd);
}

int
main(int argc, char *argv[])
{
	if(argc != 3){
		fprintf(2, "Usage: find dir name\n");
		exit(1);
	}
	find(argv[1], argv[2]);
	exit(0);
}	
