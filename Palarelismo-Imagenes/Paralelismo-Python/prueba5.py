# Multiprocessing
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import time
from PIL import Image, ImageOps
import numpy as np
from multiprocessing import Pool

Image.MAX_IMAGE_PIXELS = None

def process_image(img):
    # Convert the image to grayscale
    gray = ImageOps.grayscale(Image.fromarray(np.uint8(img)))
    # Convert to numpy array for manual thresholding
    np_gray = np.array(gray)
    # Apply threshold manually
    thresholded = np.where(np_gray > 127, 255, 0)
    return thresholded

def main():
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

    # Create a multiprocessing Pool and process the images in parallel
    with Pool() as p:
        imgs_thresholded = p.map(process_image, img_parts)

    # Join the thresholded parts
    top = np.hstack(imgs_thresholded[:2])
    bottom = np.hstack(imgs_thresholded[2:])
    joined = np.vstack((top, bottom))

    # End the timer and print the elapsed time
    end_time = time.time()
    print(f"Elapsed time: {end_time - start_time} seconds")

    plt.imshow(joined, cmap='gray')
    plt.show()

if __name__ == '__main__':
    main()