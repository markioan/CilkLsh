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

#include <stdlib.h>
#include <stdio.h>
#include "perturbation_sets.h"
#include "mex.h"
void search_queries(int q ,int N, int d ,int **cell,int *candidates, float *dataset,float *array,int *Buckets_indeces,float *datasetNF, int T,int max_index,int *cand_number,int *total_cands){
//in-function variables
int i,j,n_Buckets,sum=0,temp_index;
int *q_keys,*q_capacity;
float temp[d],*p_vector;
int value,value2,u;
temp[0:d]=0;


//allocate memory for query keys
q_keys=(int*)malloc(q*T*sizeof (int)) ;

//Initiallize q_capacity with ones
q_capacity=(int*)malloc(q*T*sizeof (int)) ;

//petrubation vector
p_vector=(float*)malloc(q*T*d*sizeof(float));

for(i=0;i<q*T;i++){
	q_capacity[i]=1;
}


//create perturbation vectors
for(i=0;i<q;i++){
	perturbation_sets(dataset,datasetNF,i,N,d,T,p_vector);
}

int summ=0;
// produce query keys
cilk_lshhash (p_vector,q_capacity,q_keys,q*T,N,d);
//printf("check \n");
// search for every query
for(i=0;i<q;i++){
	//printf("*******query no %d \n",i+1);
	total_cands[i]=0;
	
	for(u=0;u<T;u++){
		//printf("*******probing bin no %d \n",u+1);
		//printf("qkey %d \n",q_keys[(i*T)+u]);
        	candidates[(i*T)+u]=0;
		value2=0;
                //printf("checkv3 \n");
		if ((q_keys[(i*T)+u]>max_index)|| (q_keys[(i*T)+u]<0)) {
			//printf("Code No %d does not belong to this dataset \n",i+u);
			candidates[(i*T)+u]=-1;
			//cand_number[i]=0;
		}
		else {  
			for(j=0;j<cell[q_keys[(i*T)+u]][0];j++){
                		//printf("qkey %d \n",q_keys[i]);
				temp_index=Buckets_indeces[cell[q_keys[(i*T)+u]][j+1]];
                		//printf("temp_index %d \n",temp_index);
				temp[0:d]=p_vector[((i*T)+u)*d:d]-array[temp_index*d:d];
				value=__sec_reduce_all_zero(temp[0:d]);
				value2=value2+value;
				candidates[(i*T)+u]=candidates[(i*T)+u]+(cell[q_keys[(i*T)+u]][j+1])*value;
				total_cands[i]=total_cands[i]+value;
				summ=summ+value;
			}
		if(value2==1){
		//printf("REGISTER \n");
		cand_number[summ-1]=(i*T)+u;}
		//printf("total_cands %d  \n",total_cands[i]);
		//printf("index %d  \n",summ-1);
		//printf("position %d  \n",(i*T)+u);
		}

	}//printf("*******cands %d \n",total_cands[i]);
	//printf("*******candidates %d \n",candidates[cand_number[sum:total_cands[i]]]);
	//summ=summ+total_cands[i];
}
//printf("check 2\n");
//free allocated memory
free(q_capacity);
free(q_keys);
free(p_vector);

}
