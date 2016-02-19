%Before run this script first open the inputdata.mat and targetdata.mat in the workspace from the folder DATA
net=newff(minmax(inputsonar),[10 4],{'tansig' 'purelin'},'trainlm','learngdm','mse');
view(net);
[trainInd,valInd,testInd] = dividerand(0.70,0.15,0.15);
%net.trainParam.goal = .0004;
net=train(net,inputsonar,targetvalues);
y=net(inputsonar);
perf=perform(net,y,targetvalues);
e = targetvalues - y;
view(net);
ploterrhist(e);


