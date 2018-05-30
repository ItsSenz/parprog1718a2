#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000000
#define CUTOFF 10
#define LIMIT 50000

//inssort
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

int quicksort(double *a,int n) {
int i;
int sorted = 0;

  // check if below cutoff limit
  if (n<=CUTOFF) {
    inssort(a,n);
    return;
  }
  
  i = partition(a, n);

  // recursively sort halves
  quicksort(a,i);
  sorted = quicksort(a+i,n-i);
}
 
 



int main() {
  double *a;
  int i;
  
  
  
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

  
  

  
  

  // check sorting
  for (i=0;i<(N-1);i++) {
    if (a[i]>a[i+1]) {
      printf("Sort failed!\n");
      break;
    }
  }  

  free(a);
  
  return 0;
}
}
// με pthreads και thread pool...


