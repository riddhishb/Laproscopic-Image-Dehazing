
%% For cropping the images

for i =1:30
    i
k1 = int2str(i);
name = strcat(k1,'.png');
name = strcat('Database/clean/',name);
disp(name);
image_in = imread(name);
k = size(image_in);
border = (k(2) - k(1))/2;
image_out = image_in(1:k(1),border+1:k(2)-border,:);
% imshow(image_out);
imwrite(image_out,name);
end