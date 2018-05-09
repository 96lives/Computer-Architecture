my_out = imread('./btsout.bmp');
ans = imread('../bts-ans.bmp');

is_same = true;
diff_x = 0
diff_y = 0

for i = 1: size(ans, 1)
	for j = 1: size(ans, 2)
		for k = 1: 3
			if (my_out(i, j, k) ~= ans(i, j, k));
				is_same = false;
                diff_x = j;
                diff_y = i;
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
    imtool(my_out)
end
