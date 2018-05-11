
wlist = [542, 1279, 252, 400, 512, 640, 20, 640, 1024, 200, 1, 101, 1279, 252, 400, 512, 1024, 1, 101, 101];

hlist = [612, 861, 252, 561, 384, 480, 20, 640, 768, 200, 1, 103, 861, 252, 561,  384, 768, 1, 103, 103];

glist = [100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 23, 1, 47, 11, 32, 1, 5,25];

%% create inputs
for i = 1 : size(hlist, 2)
	in = 122 * ones(hlist(i), wlist(i), 3);
	imwrite(in, strcat('./in', int2str(i),'.bmp'));
end

%% create outputs
for i = 1 : size(hlist, 2)
    out = imread(strcat('./in', int2str(i), '.bmp'));
    gap = glist(i);
    for y = 1 : gap :  size(out, 1)
        for x = 1: size(out, 2)
            out(y, x, 1) = 255;
            out(y, x, 2) = 0;
            out(y, x, 3) = 0;
        end
    end

	 for x = 1 : gap :  size(out, 2)
        for y = 1: size(out, 1)
            out(y, x, 1) = 255;
            out(y, x, 2) = 0;
            out(y, x, 3) = 0;
        end
    end
	
	imwrite(out, strcat('./ans', int2str(i),'.bmp'));    
end
