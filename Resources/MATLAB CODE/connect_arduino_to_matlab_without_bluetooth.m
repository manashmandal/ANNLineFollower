clear all
clc 
a=serial('COM20');
fopen(a);
 

% alltogether=[0 0 0 0 0];   
% for i=1:1000
% 	y=fscanf(a);
%     b = strsplit(y,' ');
%      front(i) = str2double(b(1));
%     left(i) = str2double(b(2));
%     right(i) = str2double(b(3));
%     leftd(i) = str2double(b(4));
%     rightd(i) = str2double(b(5));
%     alltogether= [front(i) left(i) right(i) leftd(i) rightd(i)]
% end
% 	all_sensor_reading=[front;left; right;leftd;rightd];
% fclose(a);