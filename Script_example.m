%% A Simple script tha uses Cilk LSH Functions
clear
clc

loaddata;
%diary('CUDA_VS_CILK.txt')
%diary on


td = 12 ;%dimensions of the projection space
dt= td;
T=16; % Number of perturbation vectors
L=8 ; % Number of random projections
d=size(X,1);
W=829;
q=1000 ;% Number of queries

%% Initiallize pointer arrays
cands=uint64(zeros(1,L));
Bucket_indeces=uint64(zeros(1,L));
radix_sort_index=uint64(zeros(1,L));
Bucket_capacity=uint64(zeros(1,L));
total_codes=uint64(zeros(1,L));
cand_number=uint64(zeros(1,L));
total_cands=uint64(zeros(1,L));
%%

fprintf('calculate Projection Array...\n');
Is = lshVariables (L,td,d,'w',W); %Calculate the Variables for the projection


tic;
%%for every projection do
for i= 1:L 
fprintf('Projection time...\n');
Y = ( bsxfun(@minus, X'*Is(i).A, Is(i).b ) / Is(i).W); %Projection in the sub-space
Y=Y';
low = min(Y, [], 2);
binIdNonFlored = bsxfun(@minus, Y(1:dt,:),low(1:dt))';
binId = floor(binIdNonFlored);
[cands(i),Bucket_indeces(i),radix_sort_index(i),Bucket_capacity(i),total_codes(i),cand_number(i),total_cands(i)]=example(binId(1:N,:)',binId(1:N,:)',N,td,q,T,binIdNonFlored(1:N,:)',d);
end


%% clear variables you dont need
clearvars -except cands Bucket_capacity Bucket_indeces radix_sort_index total_codes q td d N X k cand_number total_cands W T ll

toc

%% gather all candidates and search for the nearest distances
[KNN]=example1(cands,Bucket_capacity,Bucket_indeces,radix_sort_index,total_codes,q,td,d,N,X,k,cand_number,total_cands,T);

%% insert path with the ground truth Neighbors
load('~/idxGT.mat');

fprintf('Cilk lsh recall \n');
compute_recall(idxGT(1:k,1:q),KNN(1:k,1:q))

%diary off

