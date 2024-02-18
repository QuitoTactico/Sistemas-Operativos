vector<vector<Pixel>> rotateImage(const vector<vector<Pixel>>& image, double angle) {
    // Convert angle from degrees to radians
    double rad = angle * M_PI / 180.0;

    // Calculate the size of the new image
    int width = image[0].size();
    int height = image.size();
    int newWidth = abs(width * cos(rad)) + abs(height * sin(rad));
    int newHeight = abs(height * cos(rad)) + abs(width * sin(rad));

    // Create the new image
    vector<vector<Pixel>> newImage(newHeight, vector<Pixel>(newWidth));

    // Calculate the center of the original and new image
    double centerX = width / 2.0;
    double centerY = height / 2.0;
    double newCenterX = newWidth / 2.0;
    double newCenterY = newHeight / 2.0;

    // Iterate over each pixel in the new image
    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            // Map the pixel in the new image back to the original image using a rotation transformation
            int oldX = centerX + (x - newCenterX) * cos(rad) + (y - newCenterY) * sin(rad); //Matriz de rotación: cos(angulo) sin(angulo) 
            int oldY = centerY - (x - newCenterX) * sin(rad) + (y - newCenterY) * cos(rad);//                     -sin(angulo) cos(angulo)

            // If the mapped location falls within the bounds of the original image, use the pixel value from the original image
            if (oldX >= 0 && oldX < width && oldY >= 0 && oldY < height) {
                newImage[y][x] = image[oldY][oldX];
            }
            // If it falls outside, set the pixel to a default value (black in this case)
            else {
                newImage[y][x] = {0, 0, 0};
            }
        }
    }

    return newImage;
}