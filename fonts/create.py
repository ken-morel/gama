import os
import sys
from threading import Thread
from PIL import Image

def get_free(img):
    w, h = img.size
    assert sum((w, h)) != 0
    t = b = l = r = False
    for x in range(w):
        try:
            _, a = img.getpixel((x, 0))
            if a > 0:
                break
        except:
            break
    else:
        t = True
    for x in range(w):
        try:
            _, a = img.getpixel((x, h-1))
            if a > 0:
                break
        except:
            break
    else:
        b = True
    for y in range(h):
        try:
            _, a = img.getpixel((0, y))
            if a > 0:
                break
        except:
            break
    else:
        l = True
    for y in range(h):
        try:
            _, a = img.getpixel((w-1, y))
            if a > 0:
                break
        except:
            break
    else:
        r = True
    return t, b, l, r

def tl(img):
    w, h = img.size
    return img.crop((1, 1, w-1, h-1))

def tr(img):
    w, h = img.size
    return img.crop((0, 1, w, h))

def br(img):
    w, h = img.size
    return img.crop((0, 0, w-1, h-1))

def bl(img):
    w, h = img.size
    return img.crop((1, 0, w-1, h-1))

def strip(url):
    img = Image.open(url)
    while True:
        t, b, l, r = get_free(img)
        try:
            if t and r:
                img = tr(img)
            if t and l:
                img = tl(img)
            if b and l:
                img = bl(img)
            if b and r:
                img = br(img)
            else:
                break
        except:
            raise
    print(img.size)
    img.save("DH.png")

WIDTH = 30

chars = tuple(map(chr, range(1, 256)))

maps = {}

dests = ("aclonica", "agbalumo", "arial", "tahoma", "lucida console", "cooper", "consolas", "algerian")

for dest in dests:
    os.mkdir(dest)
    os.chdir(dest)
    for x in chars:
        print("\r[" + ("-"*int((chars.index(x)+1) / len(chars) * WIDTH)).ljust(WIDTH) + "]", end="")
        os.system(f'magick -size 100x100 canvas:none -fill black -gravity Center -family "{dest}" -pointsize 90 -draw "text 0,0 \'{x}\'" "{ord(x)}".png')
        try:
            Thread(
                targe=strip,
                args=(f'{ord(x)}.png'),
            ).start()
        except:
            pass
    os.chdir("..")
