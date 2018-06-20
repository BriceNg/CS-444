#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "mt19937ar.c"

void sig_catch(int sig);
pthread_mutex_t fork1, fork2, fork3, fork4, fork5;
int fork12 = 0, fork23 = 0, fork34 = 0, fork45 = 0, fork51 = 0;
void confucius(void);
void seneca(void);
void voltaire(void);
void aristotle(void);
void machiavelli(void);
void eat(void);
void think(void);
int gen_number(int high, int low);

//Signal Handling
void sig_catch(int sig){
	//Fork Status Output
	printf("Catching signal %d\n", sig);
	printf("forks 1 and 2: %d\n", fork12);
	printf("forks 2 and 3: %d\n", fork23);
	printf("forks 3 and 4: %d\n", fork34);
	printf("forks 4 and 5: %d\n", fork45);
	printf("forks 5 and 1: %d\n", fork51);
	printf("KEY:\n");
	printf("0: not in use\n");
	printf("1: in use\n");
	pthread_mutex_destroy(&fork1);
	pthread_mutex_destroy(&fork2);
	pthread_mutex_destroy(&fork3);
	pthread_mutex_destroy(&fork4);
	pthread_mutex_destroy(&fork5);
	kill(0,sig);
	exit(0);
}

//Generate Random Number
//Uses mt19937ar.c Function
int gen_number(int high, int low) {
	int num = 0;
	num = (int)genrand_int32();
	num = abs(num);
	num %= (high - low);
	if (num < low)
		num = low;
	return num;
}

//Eat Function
//Philosophers Eat for 2-9 Seconds
void eat(void){
	int num = 0;
	num = gen_number(9, 2);
	sleep(num);
}

//Think Function
//Philosophers Think for 1-20 Seconds
void think(void){
	int num = 0;
	num = gen_number(20, 1);
	sleep(num);
}

//Confucius Philosopher Function
void confucius(void){
	//While Running
	for(;;)
	{
		//Start Thinking
		printf("Confucius is thinking\n");
		think();
		printf("Confucius is done thinking\n");
		//Check If Forks are available
		//Confucius needs forks 1 and 2
		while(fork51 == 1)
		{
			//If Fork #1 In Use
			printf("Confucius is waiting for fork 1\n");
			sleep(5);
        }
        while(fork23 == 1)
        {
			//If Fork #2 In Use
            printf("Confucius is waiting for fork 2\n");
            sleep(5);
        }
		//Philosopher Has Forks
		pthread_mutex_lock(&fork1);
		pthread_mutex_lock(&fork2);
		fork12 = 1;
		printf("Confucius has forks 1 and 2\n");
		
		//Eat
		printf("Confucius is eating\n");
		eat();
		printf("Confucius is done eating\n");
		
		//Put Forks Down
		pthread_mutex_unlock(&fork1);
		pthread_mutex_unlock(&fork2);
		fork12 = 0;
		printf("Confucius has put forks 1 and 2 down\n");
	}
}

//Seneca Philosopher Function
void seneca(void){
	//While Running
	for(;;)
	{
		//Start Thinking
		printf("Seneca is thinking\n");
		think();
		printf("Seneca is done thinking\n");
		
		//Seneca needs forks 2 and 3
		while(fork12 == 1)
		{
			//If Fork #2 In Use
			printf("Seneca is waiting for fork 2\n");
			sleep(5);
		}
		while(fork34 == 1)
		{
			//If Fork #3 In Use
			printf("Seneca is waiting for fork 3\n");
			sleep(5);
		}
		//Philosopher Has Forks
		pthread_mutex_lock(&fork2);
		pthread_mutex_lock(&fork3);
		fork23 = 1;
		printf("Seneca has forks 2 and 3\n");
		
		//Eat
		printf("Seneca is eating\n");
		eat();
		printf("Seneca is done eating\n");
		
		//Put Forks Down
		pthread_mutex_unlock(&fork2);
		pthread_mutex_unlock(&fork3);
		fork23 = 0;
		printf("Locke has put down forks 2 and 3\n");
	}
}

//Voltaire Philosopher Function
void voltaire(void){
	
	for(;;)
	{
		//Start Thinking
		printf("Voltaire is thinking\n");
		think();
		printf("Voltaire is done thinking\n");
		
		//Voltaire needs forks 3 and 4
		while(fork23 == 1)
		{
			//If Fork #3 In Use
			printf("Voltaire is waiting for fork 3\n");
			sleep(5);
		}
		while(fork45 == 1)
		{
			//If Fork #4 In Use
			printf("Voltaire is waiting for fork 4\n");
			sleep(5);
		}
		//Philosopher Has Forks
		pthread_mutex_lock(&fork3);
		pthread_mutex_lock(&fork4);
		fork34 = 1;
		printf("Voltaire has forks 3 and 4\n");
		
		//Eat
		printf("Voltaire is eating\n");
		eat();
		printf("Voltaire is done eating\n");
		
		//Put Forks Down
		pthread_mutex_unlock(&fork3);
		pthread_mutex_unlock(&fork4);
		fork34 = 0;
		printf("Voltaire has put down forks 3 and 4\n");
	}
}

//Aristotle Philosopher Function
void aristotle(void) {
	
	for(;;)
	{
		//Start Thinking
		printf("Aristotle is thinking\n");
		think();
		printf("Aristotle is done thinking\n");
		
		//Aristotle needs forks 4 and 5
		while(fork34 == 1)
		{
			//If Fork #4 In Use
			printf("Aristotle is waiting for fork 4\n");
			sleep(5);
		}
		while(fork51 == 1)
		{
			//If Fork #5 In Use
			printf("Aristotle is waiting for fork 5\n");
			sleep(5);
		}
		//Philosopher Has Forks
		pthread_mutex_lock(&fork4);
		pthread_mutex_lock(&fork5);
		fork45 = 1;
		printf("Aristotle has forks 4 and 5\n");
		
		//Eat
		printf("Aristotle is eating\n");
		eat();
		printf("Aristotle is done eating\n");
		
		//Put Forks Down
		pthread_mutex_unlock(&fork4);
		pthread_mutex_unlock(&fork5);
		fork45 = 0;
		printf("Aristotle has put down forks 4 and 5\n");
	}
}

//Machiavelli Philosopher Function
void machiavelli(void) {
	
	for(;;)
	{
		//Start Thinking
		printf("Machiavelli is thinking\n");
		think();
		printf("Machiavelli is done thinking\n");
		
		//Machiavelli needs forks 5 and 1
		while(fork45 == 1)
		{
			//If Fork #5 In Use
			printf("Machiavelli is waiting for fork 5\n");
			sleep(5);
		}
		while(fork12 == 1)
		{
			//If Fork #1 In Use
			printf("Machiavelli is waiting for fork 5\n");
		sleep(5);
		}
		//Philosopher Has Forks
		pthread_mutex_lock(&fork5);
		pthread_mutex_lock(&fork1);
		fork51 = 1;
		printf("Machiavelli has forks 5 and 1\n");
		
		//Eat
		printf("Machiavelli is eating\n");
		eat();
		printf("Machiavelliis done eating\n");
		
		//Put Forks Down
		pthread_mutex_unlock(&fork5);
		pthread_mutex_unlock(&fork1);
		fork51 = 0;
		printf("Machiavelli has put down forks 5 and 1\n");
	}
}

int main(int argc, char **argv) {
	//Initialize pthreads
	struct sigaction sig;
	pthread_t confucius_thread;
	pthread_t seneca_thread;
	pthread_t voltaire_thread;
	pthread_t aristotle_thread;
	pthread_t machiavelli_thread;
	void* confucius_func = confucius;
	void* seneca_func = seneca;
	void* voltaire_func = voltaire;
	void* aristotle_func = aristotle;
	void* machiavelli_func = machiavelli;
	
	//Signals
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = sig_catch;
	sigaction(SIGINT, &sig, NULL);
	
	//Initialize Fork Mutexes
	pthread_mutex_init(&fork1, NULL);
	pthread_mutex_init(&fork2, NULL); 
	pthread_mutex_init(&fork3, NULL); 
	pthread_mutex_init(&fork4, NULL); 
	pthread_mutex_init(&fork5, NULL); 
	
	//Create Philosopher Threads
	pthread_create(&confucius_thread, NULL, confucius_func, NULL);
	pthread_create(&seneca_thread, NULL, seneca_func, NULL);
	pthread_create(&voltaire_thread, NULL, voltaire_func, NULL);
	pthread_create(&aristotle_thread, NULL, aristotle_func, NULL);
	pthread_create(&machiavelli_thread, NULL, machiavelli_func, NULL);
	
	//Run Program
	for(;;)
	{

    }
}