/*
	Copyright (C) 2014 Giannis Markopoulos <giannarasoo7@gmail.com>. All Rights Reserved.
	
	This file is part of CilkLsh.
	CilkLsh is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with CilkLsh.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cilk_unique.c"
#include "cilk_lshhash.c"
#include "inverse_hashing.c"
#include "search_queries.c"
#include <sys/time.h>
#include "mex.h"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[]){
//int main(){
int N ,*C; float *array; int d,q,dd; float *dataset,*datasetNF;
int total_codes,ihash_size,sum=-1,tempid,j,temp;
int *Bucket_capacity,*Buckets_indeces,*radix_sort_index,*ihash,*temp_index;
int *keys,k,*candidates,T,*cand_number,*total_cands;
float t;
double time;

 //init inputs
   array = (float*)mxGetData(prhs[0]);
   dataset = (float*)mxGetData(prhs[1]);
   N = mxGetScalar(prhs[2]);
   d = mxGetScalar(prhs[3]);
   q = mxGetScalar(prhs[4]);
   T = mxGetScalar(prhs[5]);
   datasetNF = (float*)mxGetData(prhs[6]);
   dd = mxGetScalar(prhs[7]);

//***** MATLAB *******//


//init outputs
plhs[0] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS,0);
plhs[1] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS,0);
plhs[2] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS,0);
plhs[3] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS,0);
plhs[4] = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS,0);
plhs[5] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS,0);
plhs[6] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS,0);

Bucket_capacity=(int *)malloc((N)* sizeof(int));
Buckets_indeces=(int *)malloc((N)* sizeof(int));
radix_sort_index=(int *)malloc((N)* sizeof(int)); 

struct timeval startwtime, endwtime ,all,endtime;

gettimeofday(&all, NULL);
gettimeofday(&startwtime, NULL);

total_codes=cilk_unique(array,Bucket_capacity,Buckets_indeces,N,d,radix_sort_index);

gettimeofday(&endwtime, NULL);

time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("cilk_unique time %f\n", time);



printf("total_codes %d \n:",total_codes);
int check;
int ll;
Bucket_capacity = realloc(Bucket_capacity,total_codes*sizeof(int));
Buckets_indeces = realloc(Buckets_indeces,total_codes*sizeof(int));

//check=__sec_reduce_add(Bucket_capacity[0:total_codes]);
//printf("CHECKING BUCKETS %d \n",check);


keys=(int *)malloc((total_codes)* sizeof(int));
ihash=(int *)malloc((total_codes)*sizeof(int));
temp_index=(int *)malloc((total_codes)*sizeof(int));


//printf("Unique Bucket id %d with capacity:%d\n",Buckets_indeces[0:total_codes]+1,Bucket_capacity[0:total_codes]);

gettimeofday(&startwtime, NULL);

cilk_lshhash (array,Bucket_capacity,keys,total_codes,N,d);

gettimeofday(&endwtime, NULL);

time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("cilk_lshhash time %f\n", time);

int max;
max=__sec_reduce_max (keys[0:total_codes]);
printf("max key is = %d\n",max);
int i,qq;


int **cell=(int **)malloc((max+1)* sizeof(int*));

gettimeofday(&startwtime, NULL);

//second level hashing to speed up bucket search
ihash_size=inverse_hashing(temp_index,keys,total_codes,ihash);

// initiallize cell array
for (i=0;i<keys[temp_index[0]];i++){
	cell[i]=(int*)malloc(1*sizeof(int));
	cell[i][0]=0;
	}


	
int kk;
// collect the indeces of the same keys in a cell array 
sum=-1;
for(i=0;i<ihash_size;i++){
	sum=sum+ihash[i];
        //printf("ihash %d \n",ihash[i]);
        tempid=keys[temp_index[sum]];
        //printf("tempid %d \n",tempid);
        cell[tempid]=(int *)malloc((ihash[i]+1)*sizeof(int));
        //in the first position of the array we keep its length
        cell[tempid][0]=ihash[i];//+temp;
        for (j=0;j<ihash[i];j++){
        	//printf("sum %d \n",sum-j);
        	//printf("buckets %d \n" ,Buckets_indeces[temp_index[sum-j]]);
        	cell[tempid][j+1]=temp_index[sum-j];
    		}
	// initiallize cell array with zeros for keys tha do not exist
	if(((keys[temp_index[MIN(sum,total_codes-2)+1]]-keys[temp_index[sum]])>1)&&(sum!=total_codes-1)){
		for(kk=1+keys[temp_index[sum]];kk<keys[temp_index[sum+1]];kk++){
			//printf("prosecing possesion %d of cell array\n",kk);
                	cell[kk]=(int*)malloc(1*sizeof(int));
			cell[kk][0]=0;
			}
		}					
    
}

//checkout
/*
int lk;
for(i=0;i<total_codes;i++){
printf("\n");
printf("key %d \n ",keys[temp_index[i]]);
printf("number of buckets  %d\n",cell[keys[temp_index[i]]][0]);
for ( j=0;j<cell[keys[temp_index[i]]][0];j++){
    printf("Bucket index %d, ", Buckets_indeces[cell[keys[temp_index[i]]][(j+1)]]+1);
    printf("Bucket Capacity %d, ", Bucket_capacity[cell[keys[temp_index[i]]][(j+1)]]);
    for(lk=0 ; lk<Bucket_capacity[cell[keys[temp_index[i]]][(j+1)]];lk++){
    printf("Index based on non-sorted dataset %d \n", radix_sort_index[lk+Buckets_indeces[cell[keys[temp_index[i]]][(j+1)]]]/d+1);}
}
}*/

gettimeofday(&endwtime, NULL);

time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("cilk_ihash time %f\n", time);




// query time
candidates=(int*) malloc(q*T*sizeof(int));
cand_number=(int*) malloc(q*T*sizeof(int));
total_cands=(int*) malloc(q*sizeof(int));
printf(" %d Perturbation Vectors for Multi-probe function\n ",T-1);

gettimeofday(&startwtime, NULL);
search_queries(q,N,d ,cell,candidates,dataset,array,Buckets_indeces,datasetNF,T,max,cand_number,total_cands);
gettimeofday(&endwtime, NULL);

time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("cilk_search time %f\n", time);

/*
//checkout candidate indeces 
printf("dasdasdadasdas %d \n ",q);
for(i=0;i<q;i++){
printf("query NO# %d\n",i+1);
printf("Index based on non-sorted dataset \n");
printf(" Number of indeces %d\n",Bucket_capacity[candidates[i]]);
/*for(j=0;j<Bucket_capacity[candidates[i]];j++){

printf(" %d,",radix_sort_index[j+Buckets_indeces[candidates[i]]]/d+1);
}

printf("\n");
}
//printf("bucket index %d \n",radix_sort_index[Buckets_indeces[candidates[0:q]]]/d+1);

*/
int summ=0;
//checkout candidate indeces 
for(i=0;i<q;i++){
//printf("*******cands %d  for query %d \n",total_cands[i],i+1);
//printf("*******candidates %d \n",candidates[i*T:T]);

//printf("*******candidates fr0m indeces %d \n",candidates[cand_number[summ:1]]);
summ=summ+total_cands[i];

}

gettimeofday(&endtime, NULL);

time = (double)((endtime.tv_usec -all.tv_usec)/1.0e6 + endtime.tv_sec - all.tv_sec);


mexPrintf("Construction and search Time %f\n", time);








//check=__sec_reduce_add(total_cands[0:q]);

//printf("total_cands %d\n",check);

// prepare for outputs
int** ptr_1,** ptr_2,** ptr_3,** ptr_4,*ptr_5,** ptr_6,**ptr_7;//matlab pointers

//printf("Bucket pointer is %d\n",Buckets_indeces[0:total_codes]);

//******** MATLAB POINTERS ********//
ptr_1=(int **)mxGetData(plhs[0]);
ptr_2=(int **)mxGetData(plhs[1]);
ptr_3=(int **)mxGetData(plhs[2]);
ptr_4=(int **)mxGetData(plhs[3]);
ptr_5=(int *)mxGetData(plhs[4]);
ptr_6=(int **)mxGetData(plhs[5]);
ptr_7=(int **)mxGetData(plhs[6]);
*ptr_3=radix_sort_index;
*ptr_2=Buckets_indeces;
*ptr_1=candidates;
*ptr_4=Bucket_capacity;
*ptr_5=total_codes;
*ptr_6=cand_number;
*ptr_7=total_cands;
// free allocated memory and return
free(keys);
free(ihash);
free(temp_index);
for(i=0;i<max+1;i++){
	free(cell[i]);}
free(cell);
mexPrintf("exit\n");
}
