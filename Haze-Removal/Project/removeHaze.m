%function [I J T A L] = removeHaze( imageName, patch_size )

function [Orig_image out dark_ch Tx_map T A] = removeHaze( imageName, patch_size )

%REMOVEHAZE Summary of this function goes here
%   Detailed explanation goes here
    
    imageName = strcat('Database/',imageName);

    % Used to make image more realistic due to seeming unbelievable
    % when there is no sense of depth
    aerialPerspective = 0.95;

    Orig_image = imread(imageName);
     Orig_image = imresize(Orig_image,0.25);
     
    if max(max(max(size(Orig_image)))) > 768
        scale = 768 / max(max(max(size(Orig_image))));
        Orig_image = imresize(Orig_image,scale);
    end
    Orig_image = double(Orig_image) ./ 255;
    
    
    % We generate the dark channel prior at every pixel, using window size
    % and zero padding
    
    dark_ch = makeDarkChannel(Orig_image,patch_size);
    
    %   Estimate Atmosphere
    
    %  We first pick the top 0.1% bright- est pixels in the dark channel.
    %  These pixels are most haze- opaque (bounded by yellow lines in 
    %  Figure 6(b)). Among these pixels, the pixels with highest intensity 
    %  in the input image I is selected as the atmospheric light. 
    
    %
    % TL;DR  TAKE .1% of the brightest pixels
    dimJ = size(dark_ch);
    numBrightestPixels = ceil(0.001 * dimJ(1) * dimJ(2)); % Use the cieling to overestimate number needed
    
    A = estimateA(Orig_image,dark_ch,numBrightestPixels);
    
    % Section 4.1
    %   Estimate the Transmission
    %   Equation 12

    Tx_map = 1 - aerialPerspective*makeDarkChannel(Orig_image./A,patch_size);

   % [T] = generateLaplacian(I,T_est);
%     [L T] = generateLaplacian(I,3, T_est);
     T = guided_filter(Tx_map, rgb2gray(Orig_image), 0.003, 3);
    dehazed = zeros(size(Orig_image));

    % Equation 16
    for c = 1:3
         dehazed(:,:,c) = (Orig_image(:,:,c) - A(:,:,c))./(max(T, 0.1)) + A(:,:,c);
    end
    
     Orig_image = imresize(Orig_image,4);
     out = imresize(dehazed,4);
    
end

