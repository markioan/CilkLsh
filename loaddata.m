% A script generating all the data we need
%
% Date:
% March 2014
% Author:
% Giannis Markopoulos

clear all
close all

% load data
% insert your path here
load('~/datasets/sift1m.mat');

%number of points we want to use
N = min(2^20, size(x.corpus,1));
X = x.corpus(1:N,:)';
clear x;

d = size(X,1); % dimensions of corpus

k=32; % Number of neighboors

% insert your path here
%load ('~/gpuids.mat');

%calculate ground truth knn if you did not before
%[~,gpuids] = knn(X(:,1:100),X,k);
