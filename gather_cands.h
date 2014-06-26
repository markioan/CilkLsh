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

int gather_cands(int *cell,int sum){
// find and keep only the unique candidates    
//*** LOCAL VARIABLES ***//

int i;
int *temp_index;
int *buffer;
int temp,temp2;

// MEMORY ALLOCATIONS //
temp_index=(int*)malloc(sum*(sizeof(int)));

//initiallize variables
temp_index[0]=0;
temp2=1;
//find uniques
for(i=0;i<sum-1;i++){
temp=cell[i+1]-cell[i];
	if(temp!=0){
		temp2=temp2+1;
		temp_index[temp2-1]=i+1;	
	}
}

//keep only what you need
buffer=(int*)malloc(temp2*(sizeof(int)));
buffer[0:temp2]=cell[temp_index[0:temp2]];
cell[0:temp2]=buffer[0:temp2];
// free memory and return
free(temp_index);
free(buffer);
return temp2;
}
