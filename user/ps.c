
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


/*
Возвращает список процессов. 
Сам выделяет память malloc-ом, нужно не забывать вызывать free!
Если передан нулевой указатель, не создаёт массив id процессов,
а просто возвращает их количество.
*/
int get_ps_list(int** pids){
	int proc_count = ps_list(0, (int*) 0);
	if(proc_count < 0){
		printf("system error");
		exit(-1);
	}
	if(pids == 0)
		return proc_count;
	*pids = malloc(proc_count * sizeof(int));
	ps_list(proc_count, *pids);
	return proc_count;
}

/*
По номеру состояния процесса возвращает строку - человеческое название этого состояния
*/
const char* get_proc_state_str(int n){
	if(n==0) return "UNUSED";
	if(n==1) return "USED";
	if(n==2) return "SLEEPING";
	if(n==3) return "RUNNABLE";
	if(n==4) return "RUNNING";
	if(n==5) return "ZOMBIE";
	return "SYSTEM ERROR: WRONG VALUE";
}

/*
По pid процесса запрашивает у системы информацию о нём,
а затем, если процесс используется, выводит её.
*/
void print_proc(int pid){
	struct process_info info;
	ps_info(pid, &info);
	if(info.state == 0)
		return;

	printf("pid: %d\n", pid);
	printf("state: %s\n", get_proc_state_str(info.state));
	printf("parent_id: %d\n", info.parent_id);
	printf("memory: %d bytes\n", info.memory);
	printf("open files: %d\n", info.files);
	printf("ticks from start: %d\n", uptime() - info.ticks0);
	printf("running ticks: %d\n", info.running_ticks);
	printf("switch times: %d\n", info.switch_times);
	printf("name: %s\n", info.name);
}


void
main(int argc, const char *argv[]) {
	if(argc != 2){
		printf("wrong args count\n");
		exit(-1);
	}


	if(!strcmp(argv[1], "count")){
		printf("%d\n", get_ps_list((int**)0));
		exit(0);
	}

	if(!strcmp(argv[1], "pids")){
		int *pids; int proc_count = get_ps_list(&pids);

		for(int i=0; i<proc_count; i++)
			printf("%d ", pids[i]);
		printf("\n");

		free(pids); exit(0);
	}

	if(!strcmp(argv[1], "list")){
		int *pids; int proc_count = get_ps_list(&pids);
		for(int i=0; i<proc_count; i++){
			print_proc(pids[i]);
			printf("\n");
		}
		free(pids); exit(0);
	}
	
	printf("wrong argument\n");
	exit(-1);
}
