
%% Run This

clear;
p = zeros(15*589824,1);
for i=1:15
    i
    k1 = int2str(i);
name = strcat(k1,'.png');
name = strcat('veryclean/',name);
name_in = strcat(k1,'-in.png');
name_in = strcat('results/',name_in);
name_out = strcat(k1,'-out.png');
name_out = strcat('results/',name_out);

 tic;
[a,b,c,d,e,f] = removeHaze(name,21);
 toc
 p(1+(i-1)*589824:i*589824) = c(:);

%  imwrite(a,name_in);
%  imwrite(b,name_out);
% toc;

end
