% You can use this skeleton or write your own.
% You are __STRONGLY__ suggest to run this script section-by-section using Ctrl+Enter.
% See http://www.mathworks.cn/cn/help/matlab/matlab_prog/run-sections-of-programs.html for more details.

%%load data
load('data');
all_x = cat(2, x1_train, x1_test, x2_train, x2_test);
range = [min(all_x), max(all_x)];
train_x = get_x_distribution(x1_train, x2_train, range);
test_x = get_x_distribution(x1_test, x2_test, range);

%% Part1 likelihood: 
l = likelihood(train_x);

bar(range(1):range(2), l');
xlabel('x');
ylabel('P(x|\omega)');
axis([range(1) - 1, range(2) + 1, 0, 0.5]);

test_error = 0;
for i=1:size(l,2)
    if l(1,i) < l(2,i)
        test_error = test_error + test_x(1,i);
    elseif l(1,i) > l(2,i)
        test_error = test_error + test_x(2,i);
    end
end

fprintf("test error is: %d\n.", test_error);

%% Part2 posterior:
p = posterior(train_x);

bar(range(1):range(2), p');
xlabel('x');
ylabel('P(\omega|x)');
axis([range(1) - 1, range(2) + 1, 0, 1.2]);

test_error = 0;
for i=1:size(p,2)
    if p(1,i) < p(2,i)
        test_error = test_error + test_x(1,i);
    elseif p(1,i) > p(2,i)
        test_error = test_error + test_x(2,i);
    end
end
fprintf("test error is: %d\n.", test_error);

%% Part3 risk:
risk = [0, 1; 2, 0];
bayes_risk = risk * p;
min_risk = sum(min(bayes_risk));
fprintf("Minimum risk: %f\n", min_risk);
