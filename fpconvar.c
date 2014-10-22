#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define NUM_THREADS 3

int count = 1;
int doit=1;
pthread_mutex_t count_mutex; 
pthread_cond_t count_threshold_cv;
int TCOUNT;
int COUNT_LIMIT;
void *watch_count(void *t);
void *int_count_1(void *t);
void *int_count_2(void *t);
int test_prime(int);
int main(int argc,char* argv[]){
	
	TCOUNT = atoi(argv[1]);
	COUNT_LIMIT = atoi(argv[2]);
	
	pthread_mutex_init(&count_mutex, NULL); 
	pthread_cond_init (&count_threshold_cv, NULL);
	
	int i;
	int t1 = 1;
	int t2 = 2;
	int t3 = 3;
	
	pthread_t threads[NUM_THREADS];

	pthread_create(&threads[0],NULL,watch_count,(void *)t1);
	pthread_create(&threads[1],NULL,int_count_1,(void *)t2);
	pthread_create(&threads[2],NULL,int_count_2,(void *)t3);
	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL); 
	}

	pthread_exit(NULL);

}

void *watch_count(void *t){
	int my_id = (int)t;

	pthread_exit(NULL);
}
void *int_count_1(void *t){
	int my_id = (int)t;
	int i;
	int c;
	
	while(count <= 30){
			if(doit ==1){
				pthread_mutex_lock(&count_mutex);
				count++;
				printf("prime_count(): thread 2, p = %d \n", count);
				if(test_prime(count)){
					printf("prime_count(): thread 2, find prime = %d \n", count);	
				}
			
				doit = 0;
				pthread_mutex_unlock(&count_mutex);
			}
	}
	pthread_exit(NULL);
	
}
void *int_count_2(void *t){
	int my_id = (int)t;
	int i;
	int c;
	
	while(count <= 30){
			if(doit==0){	
				pthread_mutex_lock(&count_mutex);
				count++;
				printf("prime_count(): thread 3, p = %d\n",count);
				if(test_prime(count)){
						printf("prime_count(): thread 3, find prime = %d \n", count);	
				}
				doit = 1;
				pthread_mutex_unlock(&count_mutex);
				
			}
			
	}
	pthread_exit(NULL);
	
}
int test_prime(int c){
	int i;
	int k =1;
	for(i=2;i<c;i++){
		if(c%i == 0){
			k =0;
			break;
		}
	}
	return k;

	
}
