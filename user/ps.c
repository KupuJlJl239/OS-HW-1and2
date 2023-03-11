
#include "kernel/types.h"
#include "user/user.h"


void
main(int argc, char *argv[]) {
	if(argc != 2){
		printf("wrong args count\n");
		exit(1);
	}

/*
	if(!strcmp(argv[1], "count")){
		printf("wrong argument\n");
		exit(1);
	}
*/
	int proc_count = ps_list(0, (int*) 0);

	printf("%d\n", proc_count);
	exit(0);
}
