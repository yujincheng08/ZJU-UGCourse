load('TDT2_data', 'fea', 'gnd');
% YOUR CODE HERE

nClass = length(unique(gnd));
nTotal = 50;
kAC = 0;
sAC = 0;
kMIhat = 0;
sMIhat = 0;
for i = 1:50
    label = litekmeans(fea,nClass,'Replicates',10);
    label = bestMap(gnd,label);
    kAC = kAC + length(find(gnd == label))/length(gnd);
    kMIhat = kMIhat + MutualInfo(gnd,label);

    options = [];
    options.WeightMode = 'Binary';
    W = constructW(fea,options);

    label = spectral(W, 2);
    label = bestMap(gnd,label);
    sAC = sAC + length(find(gnd == label))/length(gnd);
    sMIhat = sMIhat + MutualInfo(gnd,label);
end
disp(['Clustering with kmeans. AC=',num2str(kAC/nTotal),' NMI=',num2str(kMIhat/nTotal)]);
disp(['Clustering with spetral clustering. AC=',num2str(sAC/nTotal),' NMI=',num2str(sMIhat/nTotal)]);
