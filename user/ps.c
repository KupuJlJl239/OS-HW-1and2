
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"


void assert(int cond, const char* message){
	if(!cond){
		if(message)
			printf("%s\n", message);
		exit(-1);
	}
}

/*
Возвращает список процессов. 
Сам выделяет память malloc-ом, нужно не забывать вызывать free!
Если передан нулевой указатель, не создаёт массив id процессов,
а просто возвращает их количество.
*/
int get_ps_list(int** pids){
	int proc_count = ps_list(0, (int*) 0);
	assert(proc_count >= 0, "system error");
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
а затем, если процесс существует, выводит её.
*/
void print_proc(int pid){
	struct process_info info;
	ps_info(pid, &info);
	if(ps_info(pid, &info) != 0) 
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

void print_pte(int n, uint64 pte){

	printf("%d %x %x ", n, pte, PTE2PA(pte));
	if(pte & PTE_R)	printf("READ,");
	if(pte & PTE_W)	printf("WRITE,");
	if(pte & PTE_X)	printf("EXECUTE,");
	if(pte & PTE_U)	printf("USER MODE,");
	printf("\n");

}

void print_pagetable(uint64* pt, int v){
	for(int i = 0; i < 512; i++){
		uint64 pte = pt[i];
		if(pte & PTE_V || v){
			print_pte(i, pte);
		}
	}
}

void
main(int argc, const char *argv[]) {

	assert(argc > 1, "not enough args");


	if(!strcmp(argv[1], "count")){
		assert(argc == 2, "too many args");
		printf("%d\n", get_ps_list((int**)0));
		exit(0);
	}

	if(!strcmp(argv[1], "pids")){
		assert(argc == 2, "too many args");
		int *pids; int proc_count = get_ps_list(&pids);

		for(int i=0; i<proc_count; i++)
			printf("%d ", pids[i]);
		printf("\n");

		free(pids); exit(0);
	}

	if(!strcmp(argv[1], "list")){
		assert(argc == 2, "too many args");
		int *pids; int proc_count = get_ps_list(&pids);
		for(int i=0; i<proc_count; i++){
			print_proc(pids[i]);
			printf("\n");
		}
		free(pids); exit(0);
	}

	if(!strcmp(argv[1], "pt")){
		assert(argc > 2, "not enough args");
		if(!strcmp(argv[2], "0")){
			assert(argc > 3, "not enough args");
			int pid = atoi(argv[3]);
			int v = 0;
			if(argc == 5){
				if(!strcmp(argv[4], "-v"))
					v = 1;
				else	
					assert(0, "wrong args");
			}
			uint64* pt = malloc(512 * sizeof(uint64));		
			assert(ps_pt0(pid, pt) == 0, "wrong pid");
			print_pagetable(pt, v);
			exit(0);		
		}
		else if(!strcmp(argv[2], "1")){
			assert(argc > 3, "not enough args");
			int pid = atoi(argv[3]);
			void* addr = (void*)(uint64)atoi(argv[4]);
			int v = 0;
			if(argc == 6){
				if(!strcmp(argv[4], "-v"))
					v = 1;
				else	
					assert(0, "wrong args");
			}
			uint64* pt = malloc(512 * sizeof(uint64));		
			assert(ps_pt1(pid, addr, pt) == 0, "pagetable doesn't exist");
			print_pagetable(pt, v);
			exit(0);		
		}
		else if(!strcmp(argv[2], "2")){
			assert(argc > 3, "not enough args");
			int pid = atoi(argv[3]);
			void* addr = (void*)(uint64)atoi(argv[4]);
			int v = 0;
			if(argc == 6){
				if(!strcmp(argv[4], "-v"))
					v = 1;
				else	
					assert(0, "wrong args");
			}
			uint64* pt = malloc(512 * sizeof(uint64));		
			assert(ps_pt2(pid, addr, pt) == 0, "pagetable doesn't exist");
			print_pagetable(pt, v);
			exit(0);		
		}
	}
	
	printf("wrong argument\n");
	exit(-1);
}
