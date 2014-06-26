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

#include "cilk/cilk.h"
#include "mex.h"
#include <sys/time.h>
#include "cilk_sort.c"
#include "gather_cands.h"
#include "cilk_distance.h"
#include "knn_gather.h"
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[]){

//********* MATLAB POINTERS*********//

long int *M_ptr1;
long int *M_ptr2;
long int *M_ptr3;
long int *M_ptr4;
long int *M_ptr5;
long int *M_ptr6;
int *total_codes;
float *dataset;
double *Knn;

//initiallize inputs
M_ptr1 = (long int*)mxGetData(prhs[0]);
M_ptr2 = (long int*)mxGetData(prhs[1]);
M_ptr3 = (long int*)mxGetData(prhs[2]);
M_ptr4 = (long int*)mxGetData(prhs[3]);
M_ptr5 = (long int*)mxGetData(prhs[11]);
M_ptr6 = (long int*)mxGetData(prhs[12]);
total_codes = (int*)mxGetPr(prhs[4]); // number of hash buckets
int Q = mxGetScalar(prhs[5]);  // number of queries
int td = mxGetScalar(prhs[6]); // projection dimensions
int d = mxGetScalar(prhs[7]); // dataset's dimensions
int N = mxGetScalar(prhs[8]); // dataset's size
dataset = (float*)mxGetData(prhs[9]);
int k = mxGetScalar(prhs[10]); //number of neighbors
mwSignedIndex L = (mwSignedIndex)mxGetN(prhs[0]);// number of projections
int T= mxGetScalar(prhs[13]);
//initiallize outputs
plhs[0] = mxCreateDoubleMatrix(k, Q, mxREAL);
Knn = mxGetPr(plhs[0]);

//** MEX-FILE POINTERS******//
int *cand_number;
int *cands;
int *total_cands;
int *Buckets_indeces;
int *Bucket_capacity;
int *radix_sort_index;
int *knn;
int i,g,*size,sum,sum2,j,ll,*sum_cands,*sum_cands2;
double time;

struct timeval startwtime, endwtime;

gettimeofday(&startwtime, NULL);

// allocate memory for all candidates from all projections
int **cell2=(int **)malloc((Q)* sizeof(int*));

//allocate memory for number of candidates for every query
size=(int *)malloc((Q)* sizeof(int));

//allocate memory for distances for every query
float **dist=(float**)malloc(Q*sizeof(float*));

//allocate memory for number of candidates for every query of every projection
sum_cands=(int *)malloc((Q)* sizeof(int));
sum_cands2=(int *)malloc((Q)* sizeof(int));
//inin sum_cands array
cilk_for(i=0;i<L;i++){
sum_cands[i]=0;
sum_cands2[i]=0;
}


//gather the cands for every projection
for(g=0;g<Q;g++){
	//printf("query no # %d\n",g+1);
	sum=0;
    //preprocesing
	for(i=0;i<L;i++){
		//printf("num of Buckets %d for query %d before gather \n",sum_cands[i],g+1);
		cand_number=(int*)M_ptr5[i];
		Bucket_capacity=(int*)M_ptr2[i];
		cands=(int*)M_ptr1[i];
		total_cands=(int*)M_ptr6[i];
		//printf("projection no # %d  total cands %d \n",i+1,total_cands[g]);
		for(j=0;j<total_cands[g];j++){
			sum=sum+Bucket_capacity[cands[cand_number[sum_cands[i]+j]]];
		}
	sum_cands[i]=sum_cands[i]+total_cands[g];
	//printf("num of Buckets %d for query %d after projection %d \n",sum_cands[i],g+1,i+1);		
	}
        //printf("num of cands %d for query %d \n",sum,g+1);
	//allocate memory for candidates of query g
	cell2[g]=(int*)malloc((sum)*sizeof(int));
        size[g]=sum;}

gettimeofday(&endwtime, NULL);
time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("Pre-proccess Candidates in : %f secs\n", time);

gettimeofday(&startwtime, NULL);
for(g=0;g<Q;g++){
	sum2=0;
    //now we know each size gather cands    
	//printf("check 1 \n");
	for(i=0;i<L;i++){
		cand_number=(int*)M_ptr5[i];
		Bucket_capacity=(int*)M_ptr2[i];
		cands=(int*)M_ptr1[i];
		Buckets_indeces= (int*)M_ptr3[i];
		radix_sort_index= (int*)M_ptr4[i];
		total_cands=(int*)M_ptr6[i];
        	for(ll=0;ll<total_cands[g];ll++){
			for(j=0;j<Bucket_capacity[cands[cand_number[sum_cands2[i]+ll]]];j++){
				cell2[g][sum2+j]=radix_sort_index[j+Buckets_indeces[cands[cand_number[sum_cands2[i]+ll]]]]/td+1;
			}
			sum2=sum2+Bucket_capacity[cands[cand_number[sum_cands2[i]+ll]]];

		}
		sum_cands2[i]=sum_cands2[i]+total_cands[g];
	}//printf("mathces ??? sum2 = %d for query %d\n",sum2,g+1);
	//printf("check 55 \n");	
	// delete duplicate indices
	//printf("check 2\n");
	
}

gettimeofday(&endwtime, NULL);
time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("Gather Candidates in : %f secs\n", time);


for(i=0;i<L;i++){
        Bucket_capacity=(int*)M_ptr2[i];
	cands=(int*)M_ptr1[i];
	Buckets_indeces= (int*)M_ptr3[i];
	radix_sort_index= (int*)M_ptr4[i];
	cand_number=(int*)M_ptr5[i];	
	total_cands=(int*)M_ptr6[i];
	free(total_cands);
	free(cand_number);
        free(Bucket_capacity);
        free(cands);
        free(Buckets_indeces);
        free(radix_sort_index);
    }

gettimeofday(&startwtime, NULL);

#pragma cilk grainsize=1
cilk_for(g=0;g<Q;g++){
quickSort(cell2[g],0,size[g]-1);
//new candidate size
size[g]=gather_cands(cell2[g],size[g]);
}

gettimeofday(&endwtime, NULL);
time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("Delete duplicates in : %f secs\n", time);


for(g=0;g<Q;g++){
	dist[g]=(float*)malloc(size[g]*sizeof(float));
	}



gettimeofday(&startwtime, NULL);

////calculate euklideian distance in cilk for loop in order to speed up the process
#pragma cilk grainsize=1
cilk_for(g=0;g<Q;g++){
    cilk_distance(dataset,cell2[g],dist[g],size[g],N,td,d,g);
    //Sort distances
    quickSort2(dist[g],0,size[g]-1,cell2[g]);
    }


gettimeofday(&endwtime, NULL);
time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);

mexPrintf("Calculate and Sort Distances in : %f secs\n", time);

// return k-nearest neighbors
knn_gather(cell2,Knn,Q,size,k);

long double cand_size=0;

for(i=0;i<Q;i++){
	cand_size=cand_size+size[i];
}
long double selectivity=0;

long double Q_N=0;

selectivity=cand_size/Q;
selectivity=selectivity/N;
printf("candidate size %Lf\n ",cand_size);

//printf("Q*N size %Lf\n ",Q_N);

printf(" selectivity %Lf\n ",selectivity);


// free allocated memory and exit

free(size);
free(sum_cands);
free(sum_cands2);

// free pointer to pointer arrays
for(i=0;i<Q;i++){
    free(dist[i]);
    free(cell2[i]);
    }
free(dist);
free(cell2);


}
