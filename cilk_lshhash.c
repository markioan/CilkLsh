//Simple one-level hashing function to speed up bucket search in LSH.

#include "stdlib.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
void cilk_lshhash (float *array,int *Bucket_capacity,int *keys,int total_codes, int N, int d) {


// identity plus some prime numbers
int P[]= {1,2,5,11,17,23,31,41,47,59};
int sum=-1;
int M,i;
M=MIN(10,d);

int hpos[M]; // indices of positions udes to hash
float temp[M];
temp[0:M]=0;
hpos[0:M]=0;

for( i=1;i<M+1;i++){
  if (i%2==1){
    hpos[i-1] = (i+1)/2;}
  else{
    hpos[i-1] = d-(i/2)+1;}
}

for (i=0;i<total_codes;i++){

sum=sum+Bucket_capacity[i];
//acess only the unique rows of the array
temp[0:M]=array[(sum*d)+(hpos[0:M]-1)]*P[0:M];

keys[i]=(int)__sec_reduce_add(temp[0:M])+1;

}



//printf("Keys %d \n",(int)keys[0:total_codes]);


}
