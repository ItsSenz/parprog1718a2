// simple quicksort implementation
// compile with: gcc -O2 -Wall quicksort.c -o quicksort

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000000
#define CUTOFF 10
#define THREADS 4
#define LIMIT 50000




struct message{					
	int st;
	int fns;
	};
typedef struct message message; 



message globalBuffer[MESSAGES];																							
int global_availmsg = 0;																							

pthread_cond_t msg_in = PTHREAD_COND_INITIALIZER;					
pthread_cond_t msg_out = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void inssort(double *a,int n) {
int i,j;
double t;
  
  for (i=1;i<n;i++) {
    j = i;
    while ((j>0) && (a[j-1]>a[j])) {
      t = a[j-1];  a[j-1] = a[j];  a[j] = t;
      j--;
    }
  }
}

int partition(double *a, int n) {
  int first, middle, last;
  double p,t;
  int i,j;

  first = 0;
  middle = n-1;
  last = n/2;  
  
  // put median-of-3 in the middle
  if (a[middle]<a[first]) { t = a[middle]; a[middle] = a[first]; a[first] = t; }
  if (a[last]<a[middle]) { t = a[last]; a[last] = a[middle]; a[middle] = t; }
  if (a[middle]<a[first]) { t = a[middle]; a[middle] = a[first]; a[first] = t; }
    
  // partition (first and last are already in correct half)
  p = a[middle]; // pivot
  for (i=1,j=n-2;;i++,j--) {
    while (a[i]<p) i++;
    while (p<a[j]) j--;
    if (i>=j) break;

    t = a[i]; a[i] = a[j]; a[j] = t;      
  }
  return i;
}


void quicksort(double *a,int n) {
int i;

  // check if below cutoff limit
  if (n<=CUTOFF) {
    inssort(a,n);
    return;
  }
  
  i = partition(a, n);

  // recursively sort halves
  quicksort(a,i);
  quicksort(a+i,n-i);
  
}
void *pd_thread(void *args) {	
   int i;
   message Message;
   for (i=0;i<MESSAGES;i++) {
       pthread_mutex_lock(&mutex);
       while (global_availmsg>0) {																							
	   pthread_cond_wait(&msg_out,&mutex);
       }
																															
       globalBuffer[i].st = i;
       printf("Producer: sending msg %d\n",globalBuffer[i].st);
       global_availmsg = 1;
		
																															
       pthread_cond_signal(&msg_in);
       pthread_mutex_unlock(&mutex);
       }
	
       pthread_exit(NULL); 
}
void *cm_thread(void *args) {
    int i;
    for (i=0;i<MESSAGES;i++) {
	pthread_mutex_lock(&mutex);
	while (global_availmsg<1) {
          pthread_cond_wait(&msg_in,&mutex); 
	}
	printf("Consumer: received msg %d\n",globalBuffer[i]);
	global_availmsg = 0;
	pthread_cond_signal(&msg_out);
	pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL); 
}

int main() {
  double *a;
  int i;
  struct thread_params tp;
  pthread_t tid;
  
  
  a = (double *)malloc(N*sizeof(double));
  if (a==NULL) {
    printf("error in malloc\n");
    exit(1);
  }

  // fill array with random numbers
  srand(time(NULL));
  for (i=0;i<N;i++) {
    a[i] = (double)rand()/RAND_MAX;
  }

  
  // simple quicksort
  if(pthread_create(&tid, NULL, work, &tp) != 0) {
    printf("Error in thread creation\n");
    exit(1);
  }

  
  

  // check sorting
  for (i=0;i<(N-1);i++) {
    if (a[i]>a[i+1]) {
      printf("Sort failed!\n");
      break;
    }
  }  

  free(a);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&msg_out);
  pthread_cond_destroy(&msg_in);
  
  return 0;
}
