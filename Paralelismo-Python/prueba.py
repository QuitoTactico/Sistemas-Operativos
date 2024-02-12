import cv2
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import time
from PIL import Image
Image.MAX_IMAGE_PIXELS = None

# Start the timer
start_time = time.time()
# Read the image
img = mpimg.imread('test.jpg')

# Convert the image to grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Apply threshold
_, thresholded = cv2.threshold(gray, 127, 255, cv2.THRESH_BINARY)

# Display the thresholded image
plt.imshow(thresholded, cmap='gray')
end_time = time.time()
print(f"Elapsed time: {end_time - start_time} seconds")
plt.show()

