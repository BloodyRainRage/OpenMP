#include <omp.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

using namespace std;

omp_lock_t my_lock;

void threadFunc1() {

#pragma omp single nowait
	{
#pragma omp parallel for 
		for (int i = 0; i < 10000000; ++i)
			rand();

		int tid = omp_get_thread_num();
		cout << "[big clause] Tread " << tid << " finished\n";
	}
}

void threadFunc2() {

#pragma omp single nowait
	{
#pragma omp parallel for 
		for (int i = 0; i < 10; ++i)
			rand();

		int tid = omp_get_thread_num();
		cout << "[small clause] Tread " << tid << " finished\n";
	}
}



int main() {
	omp_init_lock(&my_lock);
	srand(time(NULL));
	setlocale(LC_ALL, "en_US.UTF-8");
	omp_set_num_threads(4);
#pragma omp parallel
	{

		threadFunc1();
		threadFunc2();
	
	}

#pragma omp barrier 
#pragma omp master 
	{
		cout << "so we are in the master thread now\n Starting lock func\n";
		
#pragma omp parallel num_threads(4)
		{
			int tid = omp_get_thread_num();
			int i, j;

			for (i = 0; i < 5; ++i) {
				omp_set_lock(&my_lock);
				printf_s("[LOCKED] \tThread %d \n", tid);
				printf_s("[UNLOCKED] \tThread %d\n", tid);
				omp_unset_lock(&my_lock);
			}
		}
	}
	
	omp_destroy_lock(&my_lock);
	

}