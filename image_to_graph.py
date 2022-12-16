import cv2
import numpy as np
from math import exp, pow

FILENAME = "test1"
SIZE = 30
# Experimentally determined sigma. dependent on size
SIGMA = 30

OBJCOLOR, BKGCOLOR = (0, 0, 255), (0, 255, 0)
SOURCE, SINK = -2, -1

def save_graph_to_file(graph):
    N = len(graph)

    indices = np.nonzero(graph)
    values = graph[indices]

    print("Writing to file...")
    with open("graph_input.txt".format(FILENAME), "w") as f:
        f.write("{} {}\n".format(N, len(values)))
        for e in zip(indices[0], indices[1], values):
            f.write("{} {} {}\n".format(e[0], e[1], e[2]))



# Large when ip - iq < sigma, and small otherwise
def boundaryPenalty(ip, iq):
    bp = 100 * exp(- pow(int(ip) - int(iq), 2) / (2 * pow(SIGMA, 2)))
    return bp


def make_n_links(graph, image):
    K = -float("inf")
    r, c = image.shape
    for i in range(r):
        for j in range(c):
            x = i * c + j
            if i + 1 < r: # pixel below
                y = (i + 1) * c + j
                bp = boundaryPenalty(image[i][j], image[i + 1][j])
                graph[x][y] = graph[y][x] = bp
                K = max(K, bp)
            if j + 1 < c: # pixel to the right
                y = i * c + j + 1
                bp = boundaryPenalty(image[i][j], image[i][j + 1])
                graph[x][y] = graph[y][x] = bp
                K = max(K, bp)
    return K


def make_t_links(graph, image, seeded_image, K):
    r, c, _ = seeded_image.shape
    links = 0

    for i in range(r):
        for j in range(c):
            x = i * c + j
            # if (abs(seeded_image[i, j] - OBJCOLOR) <= 20).all():
            # obj color is red, bkg color is green. format is bgr
            if seeded_image[i, j, 2] - 20 >= seeded_image[i, j, 0]:
                # graph[x][source] = K
                graph[SOURCE][x] = K
                links += 1
            elif seeded_image[i, j, 1] - 20 >= seeded_image[i, j, 0]:
                graph[x][SINK] = K
                links += 1
                # graph[sink][x] = K
            elif (seeded_image[i, j, 0] != seeded_image[i, j, 1]):
                # print(seeded_image[i, j])
                pass
            # else:
            #     graph[x][source] = LAMBDA * regionalPenalty(image[i][j], BKG)
            #     graph[x][sink]   = LAMBDA * regionalPenalty(image[i][j], OBJ)

    print("Generated {} t-links".format(links))


image = cv2.imread("{}.jpg".format(FILENAME), cv2.IMREAD_GRAYSCALE)
image = cv2.resize(image, (SIZE, SIZE))

seeded_image = cv2.imread("{}seeded.jpg".format(FILENAME), cv2.IMREAD_COLOR)
seeded_image = cv2.resize(seeded_image, (SIZE, SIZE), interpolation=cv2.INTER_NEAREST)
unresized_seeded = cv2.resize(seeded_image, (SIZE*10, SIZE*10), interpolation=cv2.INTER_NEAREST)

V = image.size + 2
graph = np.zeros((V, V), dtype="int32")


K = make_n_links(graph, image)
make_t_links(graph, image, seeded_image, K)


save_graph_to_file(graph)

"""
cv2.imshow("image", image)
cv2.imshow("seeded image", seeded_image)
cv2.imshow("unresized seeded image", unresized_seeded)
cv2.waitKey(0)
"""
