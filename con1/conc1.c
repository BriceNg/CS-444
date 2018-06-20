#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "mt19937ar.h"

#define CPUID(EAX, EBX, ECX, EDX)\
__asm__ __volatile__("cpuid;" :\
"=a"(EAX), "=b"(EBX), "=c"(ECX), "=d"(EDX)\
 : "a"(EAX)\
);

#define _rdrand_generate(x) ({ unsigned char err; asm volatile("rdrand %0; setc %1":"=r"(*x), "=qm"(err)); err;})

//Function Prototypes
void sig_catch(int sig);
void consume(void *buff);
void produce(void *buff);
int generate_random_number(int upper_time_limit, int lower_time_limit);
void cpuid(void);

//Variables
int consumer_buffer_index;
int producer_buffer_index;
unsigned int eax,ebx,ecx,edx;
//Mutex
pthread_cond_t consumer_condition, producer_condition;

//Buffer item
struct buffer_item {
	int number;
	int sleep_time; 
};

//Create buffer of 32 Slots
struct buffer_list {
	struct buffer_item buffer[32];
	pthread_mutex_t lock;
};

struct buffer_list buffer;

//CPUID Function
void cpuid(void)
{
	CPUID(eax,ebx,ecx,edx);
}

void sig_catch(int sig)
{
	printf("Catching signal %d\n", sig);
	pthread_mutex_destroy(&buffer.lock);
	pthread_cond_destroy(&consumer_condition);
	pthread_cond_destroy(&producer_condition);
	kill(0,sig);
	exit(0);
}

int generate_random_number(int upper_time_limit, int lower_time_limit)
{
	int num = 0;
	eax = 0x01;
	cpuid();
	//If Supports rdrand, Use it
	if(ecx & 1<<30) 
	{
		_rdrand_generate(&num);
	}
	else
	{
		//Use mt19937ar.h Otherwise
		num = (int)genrand_int32();
	}
	num = abs(num);
	num %= (upper_time_limit - lower_time_limit);
	if(num < lower_time_limit)
		num = lower_time_limit;
    return num;
}

//Consumer Thread Function
void consume(void *buff)
{
	int value;
	int time_value;
	struct buffer_item from_buffer;

	pthread_mutex_lock(&buffer.lock);
	while(producer_buffer_index == 0) {
		pthread_cond_wait(&consumer_condition, &buffer.lock);
	//Get lock
	//Get Number From buffer item
	from_buffer = buffer.buffer[consumer_buffer_index];
	consumer_buffer_index++;
	if(consumer_buffer_index >= 32)
		consumer_buffer_index = 0;
	value = from_buffer.number;
	time_value = from_buffer.sleep_time;

	sleep(time_value);
	printf("Value: %d\n",value);
	pthread_cond_signal(&producer_condition);
	//Release Lock
	pthread_mutex_unlock(&buffer.lock);
}

//Producer Thread Function
void produce(void *buff)
{
	struct buffer_item slot;
	int buffer_sleep_time;
	int buffer_number;
	//Get Lock
	for(;;) {
		pthread_mutex_lock(&buffer.lock);
		while(producer_buffer_index == 31)
			pthread_cond_wait(&producer_condition, &buffer.lock);
		//Generate sleep number
		//Generate other numbers
		buffer_sleep_time = generate_random_number(9,2);
		buffer_number = generate_random_number(100,1);
		//Enter numbers into buffer
		slot.number = buffer_number;
		slot.sleep_time = buffer_sleep_time;
		buffer.buffer[producer_buffer_index] = slot;
		producer_buffer_index++;
		if(producer_buffer_index >= 32)
			producer_buffer_index = 0;
		pthread_cond_signal(&consumer_condition);
		//Release Lock
		pthread_mutex_unlock(&buffer.lock);
	}
}

int main(int argc, char **argv)
{
	pthread_t consumer, consumer2;
	pthread_t producer, producer2;
	void* consume_func = consume;
	void* produce_func = produce;
	struct sigaction sig;
	consumer_buffer_index = 0;
	producer_buffer_index = 0;

	//Initialize signal catching
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	sig.sa_handler = sig_catch;
	sigaction(SIGINT, &sig, NULL);

	pthread_cond_init(&consumer_condition, NULL);
	pthread_cond_init(&producer_condition, NULL);
	pthread_mutex_init(&buffer.lock, NULL);
	pthread_create(&producer, NULL, produce_func, NULL);
	pthread_create(&producer, NULL, produce_func, NULL);
	//Loop Forever, or Until Signal
	for(;;) {
		pthread_create(&consumer, NULL, consume_func, NULL);
		pthread_create(&consumer2, NULL, consume_func, NULL);
		pthread_join(consumer, NULL);
	}
}