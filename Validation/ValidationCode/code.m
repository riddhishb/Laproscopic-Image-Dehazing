
%% Validation of the image haze model

% Riddhish Bhalodia and Alankar Kotwal

input_image = (imread('hazefree.png'));

% The atmospheric light and the transmission map are taken from one of the
% processed hazy image, adn as the code is for images which are scaled by
% 1/4 we need to scale the test image too before applying the A and tx onto
% it. Then we will scale the output back to original and run it throught
% the algorithm and we will weather the predicted tx and A are the same as
% that of the applied ones.
% input_image = imresize(input_image,0.25);
k = size(input_image);

input_image = double(input_image)./255;

atmos_light = load('data/5A.mat');
tx_map = load('data/5tx.mat');

A = imresize(atmos_light.f,1);
tx = imresize(tx_map.d,1);
haze_image_estimate = zeros(size(input_image));

for c = 1:3
    haze_image_estimate(:,:,c) = (input_image(:,:,c) - A(:,:,c)).*(max(tx, 0.1)) + A(:,:,c);
end

for i = 1:k(1)
    for j = 1:k(2)
        if(input_image(i,j,:) == 0)
            haze_image_estimate(i,j,:) = input_image(i,j,:);
        end    
%         if(tx(i,j)==0)
%             haze_image_estimate(i,j,:) = input_image(i,j,:);
%         end    
    end    
end

% The tx_estimate and A_estimate are obtained by running the code on the
% haze_image_estimate. Now we will compre the original and the estimated tx
% and A.
haze_image_estimate = imresize(haze_image_estimate,1);
imwrite(haze_image_estimate,'haze_image_estimate.png');
atmos_light_estimate = load('data/5A_estimate.mat');
tx_map_estimate = load('data/5tx_estimate.mat');
% 
A_estimate = atmos_light_estimate.f;
tx_estimate = tx_map_estimate.d;
% for i = 1:k(1)
%     for j = 1:k(2)
%            
%         if(tx(i,j)==0)
%             tx_estimate(i,j) = tx(i,j);
%         end    
%     end    
% end
% 
res_tx = rrmse(tx(1:720,:),tx_estimate(1:720,:));
res_A = rrmse(A(1:720,:),A_estimate(1:720,:));
