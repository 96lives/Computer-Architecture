from PIL import Image
import numpy as np
import pdb


if __name__=="__main__":
    img_path = 'result.bmp'
    img = Image.open(img_path)
    a = np.array(img.getdata()).astype(np.float32).reshape( (img.size[0],img.size[1],3) )
    pdb.set_trace()
