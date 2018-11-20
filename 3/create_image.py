from PIL import Image
import numpy as np


if __name__=="__main__":
    size = (101, 100, 3)
    a = np.zeros(size)
    a[0][0]= 0
    a[1][0] = 1
    a[0][1] =2
    a[1][1] =3
    import pdb
    pdb.set_trace()
    img = Image.fromarray(np.array(a), mode='RGB')
    img.save("{}_{}.bmp".format(size[0], size[1]))




