
%% Run This

for i=1:30
    i
    k1 = int2str(i);
name = strcat(k1,'.png');
name = strcat('clean/',name);
name_in = strcat(k1,'-in.png');
name_in = strcat('results/clean/',name_in);
name_out = strcat(k1,'-out.png');
name_out = strcat('results/clean/',name_out);

 tic;
[a,b,c,d,e,f] = removeHaze(name,21);
 toc
% save('A_estimate.mat','f');
% save('tx_estimate.mat','c')
 imwrite(a,name_in);
 imwrite(b,name_out);
% toc;

end