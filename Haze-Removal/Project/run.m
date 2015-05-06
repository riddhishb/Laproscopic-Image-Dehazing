
%% Run This
 tic;
[a,b,c,d,e,f] = removeHaze('haze_image_estimate.png',21);
 toc
save('A_estimate.mat','f');
save('tx_estimate.mat','c')
%  imwrite(c,'tx_map.png');
%  imwrite(e,'A.png');
% toc;