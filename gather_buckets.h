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

#include <stdio.h>
#include <cilk/cilk.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


void gather_buckets(int *radix_sort_indeces,int *Buckets_indeces,int *Bucket_capacity, int *buffer ,int sum,int N,int d){

int i,j=1,h=0;

Bucket_capacity[0]=1;

Buckets_indeces[0]=0;
//find unique buckets and their size
for(i=1;i<N;i++){

     if(buffer[i]==1){
      h=h+1;
      Bucket_capacity[h]=1;
      Buckets_indeces[j]=radix_sort_indeces[i];
      Buckets_indeces[j]=i;
      j=j+1;}
     else{Bucket_capacity[h]=Bucket_capacity[h]+1;}

}


}

