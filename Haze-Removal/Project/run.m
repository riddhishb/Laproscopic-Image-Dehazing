
%% Run This
 tic;
[a,b,c,d,e] = removeHaze('10.png',21);
toc
imwrite(a,'Results/10-in.png');
imwrite(b,'Results/10-out.png');

% toc;