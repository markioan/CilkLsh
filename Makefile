######### To compile modify the following lines ########

MATLAB_ROOT=/usr/local/MATLAB/R2012b

########################################################
MEX=mex -O
CC=~/cilkplus-install/bin/gcc 
MEX_INC=$(MATLAB_ROOT)/extern/include/
CILKLIB= ~/cilkplus-install/lib64

all: example.o example1.o

	$(MEX) example.o -largeArrayDims -L$(CILKLIB) -lcilkrts -o example -lmwblas -lrt 



	$(MEX) example1.o -largeArrayDims -L$(CILKLIB) -lcilkrts -o example1 -lmwblas -lrt


example.o: example.c

	$(CC) -O4 -c -fPIC example.c -I$(MEX_INC) -fcilkplus -lcilkrts -lrt 

example1.o: example1.c

	$(CC) -O4 -c -fPIC example1.c -I$(MEX_INC) -fcilkplus -lcilkrts -lrt -std=c99

clean: 

	rm -f *~ *.o 


