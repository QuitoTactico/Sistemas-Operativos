import cv2
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import time
from PIL import Image
import numpy as np
Image.MAX_IMAGE_PIXELS = None

# Start the timer
start_time = time.time()
# Read the image
img = mpimg.imread('test.jpg')

# Get the dimensions of the image
height, width, _ = img.shape

# Divide the image into 4 parts
quarter_height, quarter_width = height // 2, width // 2
img1 = img[:quarter_height, :quarter_width]
img2 = img[:quarter_height, quarter_width:]
img3 = img[quarter_height:, :quarter_width]
img4 = img[quarter_height:, quarter_width:]

# Convert each part to grayscale and apply threshold
imgs_gray = [cv2.cvtColor(i, cv2.COLOR_BGR2GRAY) for i in [img1, img2, img3, img4]]
imgs_thresholded = [cv2.threshold(i, 127, 255, cv2.THRESH_BINARY)[1] for i in imgs_gray]

# Join the thresholded parts
top = np.concatenate((imgs_thresholded[0], imgs_thresholded[1]), axis=1)
bottom = np.concatenate((imgs_thresholded[2], imgs_thresholded[3]), axis=1)
joined = np.concatenate((top, bottom), axis=0)

# Display the joined image
plt.imshow(joined, cmap='gray')

# End the timer and print the elapsed time
end_time = time.time()
print(f"Elapsed time: {end_time - start_time} seconds")

plt.show()

