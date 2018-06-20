#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define RAND (rand()%10+1)

//Mutex Declarations
pthread_mutex_t ser_lock;
pthread_mutex_t del_lock;
pthread_mutex_t ins_lock;

//Linked List Struct
//Singly Linked
struct linkList {
	int data;
	struct linkList *next;
}*head;

//Count Function
//Count to Place in List
//Return Place in List
int count(){
	struct linkList *temp;
	int i=0;
	temp = head;
	while (temp != NULL){
		temp = temp->next;
		i++;
	}
	return i;
}

//Searcher Function
void searcher () {
	//printf("Im in searcher"\n");
	struct linkList *temp;
	int del;
	//Infinite Loop
	while (1) {
		del = 1;
		//If Mutex searcher trylock Fails
		if (!pthread_mutex_trylock(&ser_lock)){
			temp = head;
			//If the List is Empty
			if (temp == NULL){
				printf("List is empty\n");
				//Still Need to Add Data, so Continue not Break
				continue;
			}
			//Add Data to List
			else {
				while (temp != NULL){
					printf("[%d] ", temp->data);
					temp = temp->next;
				}
                printf("\n");
			}
			//Unlock searcher Mutex
			pthread_mutex_unlock(&ser_lock);
		}
		//Wait
		sleep(1);
    }
}

//Inserter Function
void inserter () {
	//printf("Im in inserter"\n");
	struct linkList *temp, **tail;
	int insertValue, i, del;
	//Infinite Loop
	while (1){
		del = 1;
		//Arbitrary Max Count Length of 30
		if (count() < 30){
			//If Mutex inserter Locked
			if (!pthread_mutex_lock(&ins_lock)){
				//Insert Value into List
				temp = (struct linkList *)malloc(sizeof(struct linkList));
				insertValue = RAND;
				printf("inserting--%d\n", insertValue);
				temp->data = insertValue;
				temp->next = NULL;
				tail = &head;
				if (head == NULL){
					head = temp;
				}
				else {
					while (*tail != NULL){
						tail = &((*tail)->next);
					}
					*tail = temp;
				}
				//Unlock inserter Mutex and Wait
				pthread_mutex_unlock(&ins_lock);
				sleep(5);
			}
		}
    }
}

//Deleter Function
void deleter () {
	//printf("Im in deleter"\n");
	int deleteValue;
	struct linkList *temp, *prev;
	//Infinite Loop
	while (1){
		//Can Delete if I Have Elements
		if (count() > 1){
			//If Mutex inserter trylock Fails
			if (!pthread_mutex_trylock(&ins_lock)){
				//If searcher trylock Fails
				if(!pthread_mutex_trylock(&ser_lock)){
					temp = head;
					deleteValue = RAND;
					//As Long as List isn't Empty, Delete Elements
					while (temp != NULL) {
						if(temp->data == deleteValue){
							printf("Deleting: %d\n", deleteValue);
							if (temp == head) {
								head = temp->next;
								free(temp);
								break;
							}
							else {
								prev->next = temp->next;
								free(temp);
								break;
							}
						}
						else {
							prev = temp;
							temp = temp->next;
						}
					}
					//Unlock searcher Mutex
					pthread_mutex_unlock(&ser_lock);
				}
				//Unlock inserter Mutex
				pthread_mutex_unlock(&ins_lock);
			}
			//Wait
			sleep(2);
		}
	}
}

int main(){
	int i = 0;
	struct linkList *new;
	new = (struct linkList *)malloc(sizeof(struct linkList));
	new->data = RAND;
	head = new;
	head->next = NULL;
	
	//Threads
	pthread_t search_thread[3];
	pthread_t insert_thread[3];
	pthread_t delete_thread[3];
	
	//Make 3 processes
	for(i = 0; i < 3; ++i){
		//printf("Creating insetrter\n");
		if(pthread_create(&insert_thread[i], NULL, (void *)inserter, (void *)NULL)) {
			fprintf(stderr, "Error creating inserter thread\n");
			return 1;
		}

		//printf("Creating searcher\n");
		if(pthread_create(&search_thread[i], NULL, (void *)searcher, (void *)NULL)) {
			fprintf(stderr, "Error creating searcher thread\n");
			return 1;
		}

		//printf("Creating deleter\n");
		if(pthread_create(&delete_thread[i], NULL, (void *)deleter, (void *)NULL)) {
			fprintf(stderr, "Error creating delete thread\n");
			return 1;
		}
	}
	for(i = 0; i < 3; ++i){
		pthread_join(search_thread[i], NULL);
		pthread_join(insert_thread[i], NULL);
		pthread_join(delete_thread[i], NULL);
	}
	return 0;
}