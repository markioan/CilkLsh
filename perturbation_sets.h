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


#define MIN(a,b) (((a)<(b))?(a):(b))
// expand Multi-probe operation
void expand( int *arr,int *arr2, int idx){

int size=arr[0];
int new_var;
arr2[0]=size+1;
//printf(" new size %d \n " ,arr2[0]);
arr2[1:size]=arr[1:size];
new_var=MIN(arr[size]+1,idx);
arr2[size+1]=new_var;
//printf ( "new perturbation set %d ",arr2[1:size+1]);
//printf("\n");
};

// shift Multi-probe operation
void shift ( int *arr,int *arr2,int idx){

int size=arr[0];
int new_var;
arr2[0]=size;
arr2[1:size-1]=arr[1:size-1];
new_var=MIN(arr[size]+1,idx);
arr2[size]=new_var;
//printf ( "new perturbation set %d ",arr2[1:size]);
//printf("\n");
};

//score function
float cal_score(int *arr,float *probes){

int size=arr[0];
int i;
double score=0;

for(i=0;i<size;i++){
	score=score+probes[arr[i+1]];
	//printf(" %f \n ",probes[arr[i+1]]);
	}

return score;
};



perturbation_sets(float *dataset, float* datasetNF,int q,int N, int M,int T,float *petru_vector){


float *deltas,*probes,*scores,*temp_vector,*p_vector,max_score;
int i,*adds_subs,*indeces,*probing_queue,**indeces2,*indeces3,*temp_index;
int final_sets[T-1],size,mul,j,position;


//allocate memory space
deltas=(float*)malloc(M*sizeof(float));
probes=(float*)malloc(2*M*sizeof(float));
adds_subs=(int*)malloc(2*M*sizeof(int));
indeces=(int*)malloc(2*M*sizeof(int));
p_vector=(float*)malloc(T*M*sizeof(float));
temp_vector=(float*)malloc(M*sizeof(float));
probing_queue=(int*)malloc((1+(2*M))*sizeof(int));
scores=(float*)malloc((1+(2*T))*sizeof(float));
indeces2=(int**)malloc((1+(2*T))*sizeof(int*));
indeces3=(int*)malloc((1+(2*T))*sizeof(int));
temp_index=(int*)malloc(2*M*sizeof(int));

//initiallize arrays
for(i=0;i<M;i++){
	adds_subs[i]=-1;
	indeces[i]=i;
	indeces[M+i]=i;
	adds_subs[M+i]=1;
	temp_index[i]=i;
	temp_index[i+M]=M+i;
}





// calculate deltas
deltas[0:M]=datasetNF[q*M:M]-dataset[q*M:M];

probes[0:M]=deltas[0:M];
probes[M:M]=1-deltas[0:M];


max_score=__sec_reduce_add (probes[0:2*M])+1;
//init scores with the maximum score
scores[0:2*T+1]=max_score;
//the first original vector
p_vector[0:M]=dataset[q*M:M];


indeces3[0:2*T+1]=2*T+1;
//sort the scores
quickSort2(probes,0,(2*M)-1,temp_index) ;
int idx=0;

// in the first position we keep arrays length
//because some vectors demand more than one changes to be generated
indeces2[0]=(int*)malloc(2*sizeof(int));
indeces2[0][0]=1;
indeces2[0][1]=0;
scores[0]=max_score;
indeces3[0]=0;
final_sets[0]=0;

for(i=1;i<T-1;i++){
	//shift operation of the deleted probe
	idx=idx+1;
	indeces2[idx]=(int*) malloc((indeces2[indeces3[0]][0]+1)*sizeof(int));
	shift(indeces2[indeces3[0]],indeces2[idx],2*M-1);

	//calculate new score
	scores[idx]=cal_score(indeces2[idx],probes);
	indeces3[idx]=idx;


	//expand operation of the deleted probe
	idx=idx+1;
	indeces2[idx]=(int*) malloc((indeces2[indeces3[0]][0]+2)*sizeof(int));
	expand(indeces2[indeces3[0]],indeces2[idx],2*M-1);

	//calculate new score
	scores[idx]=cal_score(indeces2[idx],probes);
	indeces3[idx]=idx;

	// sort the new set of scores
	quickSort2(scores,0,2*T,indeces3) ;
	final_sets[i]=indeces3[0];
	scores[0]=max_score;
	}


for(i=0;i<T-1;i++){
	size=indeces2[final_sets[i]][0];
	temp_vector[0:M]=p_vector[0:M];
	for(j=0;j<size;j++){
		position=indeces[temp_index[indeces2[final_sets[i]][j+1]]];
		mul=adds_subs[temp_index[indeces2[final_sets[i]][j+1]]];
		temp_vector[position]=temp_vector[position]+mul;
	}	
	p_vector[(i+1)*M:M]=temp_vector[0:M];

}

/*for(i=0;i<T;i++){

	printf(" %d ",(int)p_vector[i*M:M]);	
	printf("\n");
	}*/

petru_vector[q*T*M:T*M]=p_vector[0:T*M];

// free allocated memory and return
free(probes);
free(scores);
free(deltas);
free(p_vector);
free(adds_subs);
free(indeces);
free(indeces3);
for (i=0;i<idx;i++){
	free(indeces2[i]);
}
free(indeces2);
free(probing_queue);
free(temp_index);
free(temp_vector);
}
