img = imread('btsout.bmp');
ans = imread('bts-ans.bmp');
imtool(img);
imtool(ans);


img_diff = img - ans;
for i = 1 : size(img, 1)
    for j = 1 : size(img, 2)
        for k = 1 : 3
            if (img_diff(i, j, k)~=0)
                i
                j
            end
        end
    end
end



