my_out = imread('./out2.bmp');
ans = imread('./answers/ans2.bmp');

is_same = true;
diff_x = [];
diff_y = [];

for i = 1: size(ans, 1)
	for j = 1: size(ans, 2)
		for k = 1: 3
			if (my_out(i, j, k) ~= ans(i, j, k));
				is_same = false;
                diff_x = [diff_x, j];
                diff_y = [diff_y, i];
			end
		end
	end
end

if (is_same)
	disp("Same");
else
	disp("Different");
    diff_x
    diff_y
end

imtool(my_out)
