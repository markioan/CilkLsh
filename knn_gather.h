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
#define MIN(a,b) (((a)<(b))?(a):(b))

void knn_gather(int **cell2 , double* knn,int Q, int *size,int k){

int i,j;
int m;
// keep the k nearest neighbors
for(i=0;i<Q;i++){
	m=MIN(k,size[i]);
	for(j=0;j<m;j++){
		knn[i*k+j]=(double)cell2[i][j];	
		}		
	}

}
