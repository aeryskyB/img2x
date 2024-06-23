import numpy as np
import argparse
from PIL import Image

parser = argparse.ArgumentParser()
parser.add_argument('img', help='input image filepath')
parser.add_argument('-v', help='increase verbosity', action='store_true')

prompt = r'''
 ---------
|    .    |
|   / \   |
|   UwU   |
|  /   \  |
| /\   /\ |
|   | |   |
|   - -   |
 ---------
'''

line = '-' * 50

placeholders = list(' .*o@')

if __name__ == '__main__':
    args = parser.parse_args()

    with Image.open(args.img) as img:
        img.load()

    assert img is not None
    
    if args.v:
        print(line)
        print(prompt)
        print(f'file selected: {args.img}')
        print(f'image shape: {img.size}')
    
    w, h = img.width, img.height
    f_w, f_h = w // 80, h // 32
    gray = img.convert('L').resize((w//f_w, h//f_h))
    gray_np = np.array(gray, dtype=int)
    if args.v:
        print(f'After grayscale + numpy conversion:', 
              f'{gray_np.dtype},' 
              f'{gray_np.shape}')
        print(line, end='\n\n')
    
    idxs = ((gray_np * len(placeholders)) / 256).astype(int)
    for i in range(idxs.shape[0]):
        for j in range(idxs.shape[1]):
            print(placeholders[idxs[i, j]], end='')
        print()


