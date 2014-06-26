#include <stdio.h>
#include <cilk/cilk.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gather_buckets.h"
float *p;

int compare (const void *const first, const void *const second)
{
    return p[*(int*)first] - p[*(int*)second];
};

void indice (int *a,int x){
int i;
cilk_for(i=0;i<x;i++){
	a[i]=i;
}
};

int cilk_unique(float *array,int *Bucket_capacity,int *Buckets_indeces,int N,int d ,int *radix_sort_index) {

// init variables
float t;
time_t start,end;
int i ;
int sum=1;

// init variables
int *indices;
int *temp,*buffer;
int *buckets;
float temp3[d],*temp2,*temp4;
//start timer
start=clock();

//memory allocation
temp=(int *)malloc((N)* sizeof(int));
temp2=(float *)malloc((N)* sizeof(float));
temp4=(float *)malloc((N)* sizeof(float));
buffer=(int *)malloc((N)* sizeof(int));

indices=(int *)malloc((N*d)* sizeof(int));

//init indeces array
/*for (i=0;i<N*d;i++){
indices[i]=i;}*/
indice(indices,N*d);


//init buffer array ( 1 if row is unique, 0 otherwise)
buffer[0:N]=0;

//initialise first element of the buffer with 1
//the first row is definitely unique
buffer[0]=1;


//in-function global pointer for the array
p=array;

//initialise a temporary array with the indeces of the last row
temp[0:N]=indices[d-1:N:d];

// perform radix sort
for (i=0;i<d;i++){
//temp4[0:N]=array[temp[0:N]];
//quickSort3(array,0,N-1,temp);
qsort (temp,N , sizeof (*temp) , compare);
temp[0:N]=temp[0:N]-1;}

//radix_sort_index keeps the sorted indeces
temp[0:N]=temp[0:N]+1;
radix_sort_index[0:N]=temp[0:N];

// unique implementation
// Implement array transposition
temp2[0:N]=array[temp[0:N]];
array[0:N:d]=temp2[0:N];

//transpose the array
for (i=0;i<d-1;i++){
temp[0:N]=temp[0:N]+1;
temp2[0:N]=array[temp[0:N]];
array[1+i:N:d]=temp2[0:N];}

//find unique rows
for(i=0;i<N-1;i++){
temp3[0:d]= array[(i+1)*d:d]-array[i*d:d];
// if temp3 is full of zeros the 2 rows have the same value
// so they are not unique
buffer[i+1]= __sec_reduce_any_nonzero (temp3[0:d]);
sum=sum+buffer[i+1];}


printf("Total Unique codes %d\n",sum);
//printf("buffer %d  in Orginal index %d \n",buffer[0:N],radix_sort_index[0:N]);

 end=clock();
 t=(end-start)/CLOCKS_PER_SEC;

//printf("the sorted indeces are (inside function) %d\n",radix_sort_index[0:N]);


gather_buckets(radix_sort_index,Buckets_indeces,Bucket_capacity,buffer,sum,N,d);

//free allocated memory and continue
free(buffer);
free(indices);
free(temp);
free(temp2);
free(temp4);
return sum;

}

