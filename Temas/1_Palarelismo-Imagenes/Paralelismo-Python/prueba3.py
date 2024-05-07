# Prueba utilizando paralelismo con hilos
import cv2
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import time
from PIL import Image
import numpy as np
Image.MAX_IMAGE_PIXELS = None

import concurrent.futures

def process_image(img):
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    _, thresholded = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)
    return thresholded

# Start the timer
start_time = time.time()

# Read the image
img = mpimg.imread('drogas.jpg')

# Get the dimensions of the image
height, width, _ = img.shape

# Divide the image into 4 parts
quarter_height, quarter_width = height // 2, width // 2
img_parts = [img[:quarter_height, :quarter_width], img[:quarter_height, quarter_width:],
             img[quarter_height:, :quarter_width], img[quarter_height:, quarter_width:]]

# Convert each part to grayscale and apply threshold in parallel
with concurrent.futures.ThreadPoolExecutor() as executor:
    imgs_thresholded = list(executor.map(process_image, img_parts))

# Join the thresholded parts
top = np.concatenate((imgs_thresholded[0], imgs_thresholded[1]), axis=1)
bottom = np.concatenate((imgs_thresholded[2], imgs_thresholded[3]), axis=1)
joined = np.concatenate((top, bottom), axis=0)

# End the timer and print the elapsed time
end_time = time.time()
print(f"Elapsed time: {end_time - start_time} seconds")

plt.imshow(joined, cmap='gray')
plt.show()
