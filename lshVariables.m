function I = lshVariables(l,k,d,varargin)
% These function sets up the LSH hashing functions and the hash 
% tables based on these functions.
% Input:
%     l : number of functions (i.e., of hash tables)
%     k : number of bits in a function (i.e., length of a key)
%     d : is the dimension of the data.
%    case 'w' : choose the bin width manually by writing 'w',a
%                where a is the value you want
% Output:
%     W : the width of intervals on random projection line
%     A : projection matrix (d x k)
%     b : random shifts (1 x k)
%

range=255;
w=[];

for a=1:2:length(varargin)
  eval(sprintf('%s=varargin{a+1};',lower(varargin{a})));
end

% convert the range to 
range = processRange(d,range);

  if (isempty(w))
     w=-16;
  end
  
  if (w < 0)
    % a rough estimate of the range of the projection
    % We have a sum of d normal RVs multiplied by the range of the data
    limits = max(abs(range(1,:)),abs(range(2,:)));
    rangeAct=mean(diff([-limits; limits]*2*sqrt(d)));
    n=abs(w);
    w = rangeAct/n;
  end
    
  for j=1:l
    I(j).W = w;
    I(j).A = randn(d,k);
    I(j).b = unifrnd(0,w,1,k);
    I(j).k = k;
    
  end
end

