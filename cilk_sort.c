// quicksort parallel implementation with cilk spawn

void quickSort(int arr[], int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2]; 
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
      /* recursion  */
	  if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
		

}

// same quicksort to change arrays indices simultaneously
void quickSort2( float arr[], int left, int right, int p[]) {
      int i = left, j = right;
      int tmp2;
      double tmp, pivot = arr[(left + right) / 2];
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                   tmp = arr[i];
		  tmp2= p[i];
                  arr[i] = arr[j];
		  p[i]=p[j];
                  arr[j] = tmp;
		  p[j]=tmp2;
                  i++;
                  j--;

            }
      };
      /* recursion  */
	  if (left < j)
            quickSort2(arr, left, j,p);
      if (i < right)
            quickSort2(arr, i, right,p);
		
		
}

void quickSort3( int arr[], int left, int right, int p[]) {
      int i = left, j = right;
      int tmp,tmp2;
      double pivot = arr[(left + right) / 2];
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
		  tmp2= p[i];
                  arr[i] = arr[j];
		  p[i]=p[j];
                  arr[j] = tmp;
		  p[j]=tmp2;
                  i++;
                  j--;
            }
      };
      /* recursion  */
	  if (left < j)
           cilk_spawn quickSort3(arr, left, j,p);
      if (i < right)
            quickSort3(arr, i, right,p);
		cilk_sync;
		
}

void quickSort4( float arr[], int left, int right, int p[]) {
      int i = left, j = right;
      int tmp2;
      float tmp, pivot = arr[(left + right) / 2];
      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                   tmp = arr[i];
		  tmp2= p[i];
                  arr[i] = arr[j];
		  p[i]=p[j];
                  arr[j] = tmp;
		  p[j]=tmp2;
                  i++;
                  j--;
            }
      };
      /* recursion  */
	  if (left < j)
           cilk_spawn quickSort4(arr, left, j,p);
      if (i < right)
            quickSort4(arr, i, right,p);
		cilk_sync;
}



