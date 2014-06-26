#include "math.h"
#include "mex.h"

void cilk_distance(float *dataset,int* indeces, float *dist,int size,int N,int td,int d,int query){
//init variables    
int i,j;


//calculate euclideian distance with vectorized operations
for (int i=0;i<size;i++){

        float s = 0;
        #pragma simd reduction(+:s)
        for(int j=0;j<d;j++){
           float g=dataset[query*d+j]-dataset[(indeces[i]-1)*d+j];
           s += g*g;
        } 
        dist[i] = s;
	}

}
