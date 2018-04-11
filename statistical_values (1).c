#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
/*Error handling for pthread_create and pthread_join*/
/*from the pthread_create man page*/
#define handle_error_en(en, msg) \
        do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
/* no of running threads */
volatile int running_threads = 0;
pthread_t thread[3]; /*Descriptors for our 3 threads*/
int no_of_elements;/*Total no of elements from the user*/
struct R{ /*Structure to hold the Results*/
	int min;
	int max;
	int average;
}R;
/*to finds the minimum element */
void *findMin(void *arrptr){
		int i; /*counter*/
	int *elements = (int*)arrptr; /*re reference void array pointer*/ 
	R.min = elements[0]; /*set minimum to first element */
	for(i = 0; i < no_of_elements; i++){	/*iterate through array*/
		if(elements[i] < R.min){	/*if the current element is less than the current min*/
		R.min = elements[i];	/*store the new min*/
		}
		}
	running_threads -= 1;	/*Decrement thread count*/
return NULL;
}
/*This function finds the maximum element of an array*/
void *findMax(void *arrptr){
		int i;	/*counter*/
        int *elements = (int*)arrptr; /*re reference void array pointer*/ 
		for(i = 0; i < no_of_elements; i++){	/*iterate through array*/
		if(elements[i] > R.max){	/*store the new max*/
			R.max = elements[i];
		}
	}
	running_threads -= 1;	/*Decrement thread count*/
return NULL;
}
/*This function finds the average of an array*/
void *findAverage(void *arrptr){
		int i;	 /*counter*/
	int *elements = (int*)arrptr; 	/*re reference void array pointer*/
	for(i = 0; i < no_of_elements; i++){		/*iterate through array*/
		R.average += elements[i];		/*add element @ i to average*/
	}
	R.average = R.average/no_of_elements;	/*Divide the sum by the number of elements*/
	running_threads -= 1;	/*Decrement running threads counter*/
return NULL;
}
/* This method accepts a int n(initial size of array) and
   pointer to an array and returns # of elements in the array
*/
int arrInput(int n, int *arrptr){
				int input;/*Store user input */
		int numberOfElements = 0;/*Number of Integers inputed*/
		for(;;){  /*infinite loop*/
 		printf("Enter a positive value to calculate statistical values:\nNegative Number to Stop\n-\n");
       		//Get Int from console, store at address of input
			if (scanf("%d",&input) != 1){
				printf("\nOops that wasn't an Integer\nlets try filling the array again\nRemember INTEGERS only!\n");
				exit(EXIT_FAILURE);
			}
    		if (input >= 0){ 
       		 	if (numberOfElements == n){
	  	  n += 1; //Make room for the current input
          	  arrptr = realloc(arrptr, n * sizeof(int));//realloc array and set pointer
                   			 }
    		arrptr[numberOfElements++] = input;//Store input at next empty element
    	} else {
              		 printf("\nNumber of Integers: %d\n", numberOfElements);
              		 break;
   				 }
			}
return numberOfElements;
			}
/*This function joins our n number of threads */
void thread_joining(int numberOfThreads){
		int i; /*count*/
	int s; /*error #*/
	while(numberOfThreads >= 0){	/*Join our threads*/
		s = pthread_join(thread[numberOfThreads], NULL);
/*if we recieve anything other than 0 we have a join error*/
		 if (s != 0){
		 	/*handle error*/
			handle_error_en(s, "pthread_create");
		 		 }
		 numberOfThreads--;
}
	}
	/*This function creates the 3 threads we need and supplys
  error catching for pthread_create, it could be 
  modified easily to create any # of threads automatically
*/
void thread_creation(int *arrptr){
		int s; /*error #*/
	 /*Create a thread and passing in the function to begin 
	 exectuing as well as that functions required arguments*/ 
  	s = pthread_create(&thread[0], NULL, findMin, (void *)arrptr);
	 if (s != 0){
			handle_error_en(s, "pthread_create");
		 		 }
		 	running_threads += 1;
	 /*Create a thread and passing in the function to begin 
	 exectuing as well as that functions required arguments*/ 
	 s = pthread_create(&thread[1], NULL, findMax, (void *)arrptr);
		 if (s != 0){
                    handle_error_en(s, "pthread_create");
       	      	 }
	        	 running_threads += 1;
	 /*Create a thread and passing in the function to begin 
	 exectuing as well as that functions required arguments*/ 
	 s = pthread_create(&thread[2], NULL, findAverage, (void *)arrptr);
	 	if (s != 0){
           handle_error_en(s, "pthread_create");
       	       	 }
						running_threads += 1;
}
/* The main function initialiazes the dynamic array as well
   as allocating space for it, Then it creates, using pthread_create,
   3 Threads 1 to calculate the min, the max, and the average.
   We then wait until each thread completes its task and then
   join the 3 threads and prompt the user with the results
 */
int main(){
	int n = 1; /* Initial Array Size*/
int *arrptr = malloc(n * sizeof(int));/*Initialize array pointer*/
		 /*get an n sized array of elements from the user and save count*/
		 no_of_elements = arrInput(n, arrptr);
		 thread_creation(arrptr);
		    	while(running_threads>0){	/*Wait for each thread to decrement*/
					sleep(1);
			}
		thread_joining(2);	/*Call our thread joining function passing # of threads */
		/*Prompt the user with our R*/
		printf("\nThe average is :%d\nThe maximum is :%d\nThe minimum is :%d\n",R.average, R.max, R.min);
	return(0);
}
