
#include "kernel/types.h"
#include "user/user.h"


void
main(int argc, const char *argv[]) {
	if(argc != 2){
		printf("wrong args count\n");
		exit(-1);
	}


	if(!strcmp(argv[1], "count")){
		int proc_count = ps_list(0, (int*) 0);
		printf("%d\n", proc_count);
		exit(0);
	}

	if(!strcmp(argv[1], "pids")){
		int proc_count = ps_list(0, (int*) 0);
		if(proc_count < 0){
			printf("system error");
			exit(-1);
		}

		int *pids = malloc(proc_count * sizeof(int));
		ps_list(proc_count, pids);
		for(int i=0; i<proc_count; i++)
			printf("%d ", pids[i]);
		printf("\n");
		exit(0);
	}
	
	printf("wrong argument\n");
	exit(-1);
}
