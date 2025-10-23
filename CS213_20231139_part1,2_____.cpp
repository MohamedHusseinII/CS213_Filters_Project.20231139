#include <iostream>
#include "Image_Class.h"
using namespace std;

// ===== Basic Filters =====

// Invert filter
void invertColors(unsigned char* data, int width, int height, int channels) {
    int size = width * height * channels;
    for (int i = 0; i < size; i++)
        data[i] = 255 - data[i];
}

// Rotate filter
unsigned char* rotateImage(unsigned char* data, int width, int height, int channels,
                           int angle, int& newW, int& newH) {
    if (angle % 180 == 0) {
        newW = width;
        newH = height;
    } else {
        newW = height;
        newH = width;
    }

    unsigned char* rotated = new unsigned char[newW * newH * channels];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < channels; c++) {
                int src = (y * width + x) * channels + c;
                int dst;
                if (angle == 90)
                    dst = (x * newW + (newW - y - 1)) * channels + c;
                else if (angle == 180)
                    dst = ((newH - y - 1) * newW + (newW - x - 1)) * channels + c;
                else if (angle == 270)
                    dst = ((newH - x - 1) * newW + y) * channels + c;
                else
                    dst = src;

                rotated[dst] = data[src];
            }
        }
    }
    return rotated;
}

// Crop filter
unsigned char* cropImage(unsigned char* data, int width, int height, int channels,
                         int x, int y, int w, int h) {
    unsigned char* cropped = new unsigned char[w * h * channels];
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for (int c = 0; c < channels; c++) {
                int src = ((y + i) * width + (x + j)) * channels + c;
                int dst = (i * w + j) * channels + c;
                cropped[dst] = data[src];
            }
        }
    }
    return cropped;
}

// Resize filter
unsigned char* resizeImage(unsigned char* data, int width, int height, int channels,
                           int newW, int newH) {
    unsigned char* resized = new unsigned char[newW * newH * channels];
    for (int y = 0; y < newH; y++) {
        for (int x = 0; x < newW; x++) {
            int srcX = x * width / newW;
            int srcY = y * height / newH;
            for (int c = 0; c < channels; c++) {
                resized[(y * newW + x) * channels + c] =
                    data[(srcY * width + srcX) * channels + c];
            }
        }
    }
    return resized;
}

// ===== Main Program =====
int main() {
    string filename;
    cout << "Enter image file name: ";
    cin >> filename;

    Image img;
    img.loadNewImage(filename);

    int choice = 0;
    while (choice != 6) {
        cout << "\n--- MENU ---\n";
        cout << "1. Invert colors\n";
        cout << "2. Rotate\n";
        cout << "3. Crop\n";
        cout << "4. Resize\n";
        cout << "5. Save\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            invertColors(img.imageData, img.width, img.height, img.channels);
            cout << "Inverted successfully.\n";
        }
        else if (choice == 2) {
            int angle;
            cout << "Enter rotation angle (90, 180, 270): ";
            cin >> angle;

            int newW, newH;
            unsigned char* rotated = rotateImage(img.imageData, img.width,
                                                 img.height, img.channels,
                                                 angle, newW, newH);

            Image rotatedImg(newW, newH);
            rotatedImg.channels = img.channels;
            rotatedImg.imageData = rotated;

            img = rotatedImg;
            cout << "Rotated successfully.\n";
        }
        else if (choice == 3) {
            int x, y, w, h;
            cout << "Enter x, y, width, height: ";
            cin >> x >> y >> w >> h;

            unsigned char* cropped = cropImage(img.imageData, img.width,
                                               img.height, img.channels,
                                               x, y, w, h);

            Image croppedImg(w, h);
            croppedImg.channels = img.channels;
            croppedImg.imageData = cropped;

            img = croppedImg;
            cout << "Cropped successfully.\n";
        }
        else if (choice == 4) {
            int newW, newH;
            cout << "Enter new width and height: ";
            cin >> newW >> newH;

            unsigned char* resized = resizeImage(img.imageData, img.width,
                                                 img.height, img.channels,
                                                 newW, newH);

            Image resizedImg(newW, newH);
            resizedImg.channels = img.channels;
            resizedImg.imageData = resized;

            img = resizedImg;
            cout << "Resized successfully.\n";
        }
        else if (choice == 5) {
            string outname;
            cout << "Enter output file name: ";
            cin >> outname;
            img.saveImage(outname);
            cout << "Saved successfully.\n";
        }
    }

    cout << "Program finished.\n";
    return 0;
}
