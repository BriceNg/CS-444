#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MAX_SEATS 12
#define WAIT_TIME 30

//Mutex for accessing the shop
pthread_mutex_t shop_mutex;

//Mutex for cutting hair next
pthread_mutex_t nextCustomer_mutex;

//Mutex for when the barber falls asleep
pthread_mutex_t barberSleep_mutex;

//Whether the barber is sleeping
pthread_cond_t barberSleepStatus_cond;

//Whether the barber is working
pthread_cond_t barberWorkStatus_cond;

int numCustomers = 0;
int barberSleeping = 0;
int haircutTime = 0;

void get_hair_cut();
void cut_hair();
void *generate_customer();
void customer();
void *waiting_room();
void *barber_room();

//Get Hair Cut Function
//Called by Customer
void get_hair_cut(){
	printf("Customer is getting a haircut. \n");
	sleep(haircutTime);
}

 //Cut Hair Function
 //Called by Barber
void cut_hair(){
	printf("Barber is cutting hair. \n");
	sleep(haircutTime);
}

//This thread creates threads for each customer, up to the max number of seats defined plus one.
void *generate_customer(){
	int i = 0;
	pthread_t customers[MAX_SEATS + 1];
	pthread_attr_t customers_attr[MAX_SEATS + 1];

	while(1){
		sleep(rand()%6+1);
		for(i=0; i < (MAX_SEATS + 1); ++i) {
			pthread_attr_init(&customers_attr[i]);
			pthread_create(&customers[i], &customers_attr[i], waiting_room, NULL);
		}
	}
}

void customer(){
	//Check if seat is free.
	if(numCustomers < MAX_SEATS){
		//From the problem, if barber is asleep, wake him.
		if(barberSleeping == 1){
			printf("The barber is sleeping! The customer wakes him up. \n");
			pthread_cond_signal(&barberSleepStatus_cond);
		}
		//Customer mutex waits for the barber's signal to sit in seat.
		pthread_mutex_unlock(&shop_mutex);
		printf("There is a seat! The customer sits down.\n");
		pthread_mutex_lock(&nextCustomer_mutex);
		pthread_cond_wait(&barberWorkStatus_cond, &nextCustomer_mutex);
		get_hair_cut();
		pthread_mutex_unlock(&nextCustomer_mutex);
		pthread_exit(0);
		numCustomers--;
	}
    
	//If there are no available seats, the customer leaves.
	if(numCustomers >= MAX_SEATS){
		numCustomers--;
		pthread_mutex_unlock(&shop_mutex);
		printf("The barber shop is full!  The customer leaves. \n");
		pthread_exit(0);
	}
}

//Waiting Room Function
void *waiting_room(){ 
    //Customer enters shop
	pthread_mutex_lock(&shop_mutex); 
	numCustomers++; 
	printf("A customer has entered the barber shop. They look for a seat... \n");
	//Call Customer to see if seats are free.
	customer();
}

void *barber_room(){

    while(1){    
        //If no customers, the barber falls asleep
        if(numCustomers == 0){
            pthread_mutex_lock(&barberSleep_mutex);
            barberSleeping = 1;
            printf("The shop is empty. The barber falls alseep.\n");
            pthread_cond_wait(&barberSleepStatus_cond, &barberSleep_mutex);
            barberSleeping = 0;
            printf("The barber woke up!\n");
        }
        //The barber signals for the next customer to begin the haircut.
        else{
            printf("The barber calls for the next customer.\n");
            srand(time(NULL));
            haircutTime = rand()%30;
			//if not busy,
            pthread_cond_signal(&barberWorkStatus_cond);
			//cut hair
            cut_hair();
            printf("The barber finished cutting the hair!\n");
        }      
    }   
}

int main(int argc, char *argv[]){

    pthread_t barber;
    pthread_t customersGenerator;
    pthread_t timer;
    
    pthread_attr_t barber_attr;
    pthread_attr_t customersGenerator_attr;
    pthread_attr_t timer_attr;
    
    pthread_attr_init(&barber_attr);
    pthread_attr_init(&customersGenerator_attr);
    pthread_attr_init(&timer_attr);

    pthread_create(&barber, &barber_attr, barber_room, NULL);
    pthread_create(&customersGenerator, &customersGenerator_attr, generate_customer, NULL);

    pthread_join(barber, NULL);
    pthread_join(customersGenerator, NULL);

    return 0;
}