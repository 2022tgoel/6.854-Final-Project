import cv2
import numpy as np
from math import exp, pow

FILENAME = "testbaby"
SIZE = 200

OBJCOLOR, BKGCOLOR = (0, 0, 255), (0, 255, 0)
SOURCE, SINK = -2, -1


def read_cuts(filename, image):
    with open(filename, "r") as f:
        lines = f.readlines()
        mf = int(lines[0])
        idx = 0
        for char in lines[1]:
            if idx >= SIZE*SIZE:
                break
            r, c = idx // SIZE, idx % SIZE
            idx += 1
            if char == '0':
                # attached to sink
                image[r][c] = (0, 0, 255)
            else:
                # attached to source
                image[r][c] = (0, 255, 0)


image = cv2.imread("{}.jpg".format(FILENAME), cv2.IMREAD_GRAYSCALE)
image = cv2.resize(image, (SIZE, SIZE))

seeded_image = cv2.imread("{}seeded.jpg".format(FILENAME), cv2.IMREAD_COLOR)
seeded_image = cv2.resize(seeded_image, (SIZE, SIZE), interpolation=cv2.INTER_NEAREST)
unresized_seeded = cv2.resize(seeded_image, (SIZE*10, SIZE*10), interpolation=cv2.INTER_NEAREST)

V = image.size + 2
graph = np.zeros((V, V), dtype="int32")

cuts = read_cuts("graph_output.txt".format(FILENAME), seeded_image)

cv2.imshow("image", image)
cv2.imshow("seeded image", seeded_image)
cv2.imshow("unresized seeded image", unresized_seeded)
cv2.waitKey(0)
