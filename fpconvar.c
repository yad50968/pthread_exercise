#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define NUM_THREADS 3
pthread_mutex_t count_mutex; 
pthread_cond_t count_threshold_cv;

int count = 1;
int sended =0;
int lastprime=2;
int TCOUNT;
int COUNT_LIMIT;
int catchprime;
void *watch_count(void *t);
void *int_count(void *t);
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
	usleep(100);
	pthread_create(&threads[1],NULL,int_count,(void *)t2);
	usleep(100);
	pthread_create(&threads[2],NULL,int_count,(void *)t3);
	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL); 
	}
	printf("Main() : Waited and joined with %d threads. Final value of count = %d Down.\n", NUM_THREADS, count ) ; 
	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);
	pthread_exit(NULL);

}

void *watch_count(void *t){
	int my_id = (int)t;

	printf("Starting Watch_count(): thread %d, p = %d Going to wait...\n",my_id,count ) ; 
	
	pthread_mutex_lock( &count_mutex ) ;
	if( count < COUNT_LIMIT ){	
		pthread_cond_wait( &count_threshold_cv, &count_mutex ) ; 	
		printf("watch_count(): thread %d Conditional signal received. p = %d\n",my_id ,catchprime);
		printf("watch_count(): thread %d Updating the value of p...\n", my_id ) ;
		printf("the lastest prime found before p = %d\n", lastprime ) ;  
		printf("watch_count(): thread %d count p now = %d\n", my_id, catchprime = catchprime + lastprime ) ;
		printf("watch_count():thread 1 Unlocking mutex\n");
		count = catchprime;	
	}
	pthread_mutex_unlock( &count_mutex ) ;
	usleep(10000);
	pthread_exit(NULL);
}
void *int_count(void *t){
	int my_id = (int)t;
	int i;
	int c;
	while(count < TCOUNT){
		pthread_mutex_lock(&count_mutex);
		if(test_prime(count))lastprime = count;
		count++;
		printf("prime_count(): thread %d, p = %d \n", t,count);
		if(test_prime(count)){
			printf("prime_count(): thread %d, find prime = %d \n", t,count);	
			
			if(sended==0 && count == COUNT_LIMIT){
				printf("prime_count(): thread %d, prime = %d prime reached.\n", my_id, count) ;
				catchprime = count;
                	        pthread_cond_signal( &count_threshold_cv ) ;   
                	        sended ==1;
                	        printf("Just send signal.\n") ;
                		
			}
		}	
		
		if(count >=TCOUNT){
			pthread_mutex_unlock(&count_mutex);
			usleep(10000);
			break;

		}
		pthread_mutex_unlock(&count_mutex);
		usleep(10000);
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
