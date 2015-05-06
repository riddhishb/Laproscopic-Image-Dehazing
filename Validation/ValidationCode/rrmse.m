% *******************************************************************************************
% CS736 Assignment 2 Question 1
% Alankar Kotwal <alankar.kotwal@iitb.ac.in>
% RRMSE Calculator, assumes input images of the same size
% Make sure the all fucntions are in the same directory as myDriver
% Run the code after CDing to that directory.
% *******************************************************************************************

function out = rrmse(inA, inB)

if(prod(double(size(inA) == size(inB))) ~= 1)
    error('RRMSE: Image sizes are not the same');
end

out = sqrt(sum(sum(sum((abs(inA)-abs(inB)).^2))))/sqrt(sum(sum(sum(abs(inA).^2))));

end