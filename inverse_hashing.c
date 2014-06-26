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

#include "cilk_sort.c"

int inverse_hashing(int *temp_index,int *keys, int total_codes, int *ihash){
int i,l,j,tempid;
for (i=0;i<total_codes;i++){
temp_index[i]=i;}
int *keyss=(int*)malloc(total_codes*sizeof(int));
keyss[0:total_codes]=keys[0:total_codes];

//sort the keys for faster search
quickSort3 (keyss,0,total_codes-1,temp_index);

int sum=1;
ihash[0]=1;
l=0;
// check for duplicate keys
for (i=0;i<total_codes-1;i++){
    if (keys[temp_index[i]]==keys[temp_index[i+1]]){
    sum=sum+1;
    ihash[l]=sum;
    }
    else {
    sum=1;
    l=l+1;
    ihash[l]=1; 
    }

}
free(keyss);
return l+1;
}
