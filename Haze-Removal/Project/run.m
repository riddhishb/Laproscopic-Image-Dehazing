
% %% Run This

clear;
p = zeros(20*589824,1);
for i=1:50
    i
    k1 = int2str(i);
 name = strcat(k1,'.png');
%name = 'haze_image_estimate.png';
% name = strcat('clean/',name);
name_in = strcat(k1,'-in.png');
name_in = strcat('results/',name_in);
name_out = strcat(k1,'-out.png');
name_out = strcat('results/',name_out);

 tic;
[a,b,c,d,e,f] = removeHaze(name,35);
 toc
%  p(1+(i-1)*589824:i*589824) = c(:);

% save('5tx.mat','d');
%  save('5A.mat','f');
% save('5tx_estimate.mat','d');
% save('5A_estimate.mat','f');
  imwrite(a,name_in);
  imwrite(b,name_out);
% toc;

end
