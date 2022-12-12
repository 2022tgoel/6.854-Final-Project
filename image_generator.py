"""
This file isn't used...
"""

import cv2

drawing = False

def prompt_seeds(image):
	seeds = np.zeroes(image.shape, dtype="uint8")
	image = cv2.cvtColor(image, cv2.COLOR_GRAY2RG)

	SCALING_FACTOR = 10
	image = cv2.resize(image, (0, 0), fx=SCALING_FACTOR,fy=SCALING_FACTOR)

	radius = 10
	thickness = -1 # fills whole circle


	# todo this is not implemented lol
	paint_seeds(FOREGROUND)
	paint_seeds(BACKGROUND)

	return seeds, image


def build_graph(image):
	seeds, seeded_image = prompt_seeds(image)
	return None, seeded_image


image = cv2.imread("test1.jpg", cv2.IMREAD_GRAYSCALE)
image = cv2.resize(image, (30, 30))

graph, seeded_image = build_graph(image)

cv2.imshow("seeded image", seeded_image)
cv2.imshow("image", image)
cv2.waitKey(0)