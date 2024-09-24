#include <iostream>
#include <fstream>
#include "Image.hpp"
#include "processing.hpp"
using namespace std;

int main(int argc, char *argv[]) {

    // for (int i = 0; i < argc; i++) {
    //     cout << "argv[" << i << "]: " << argv[i] << endl;
    // }

    // if (argc != 5) {
    //     cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
    //  << "WIDTH and HEIGHT must be less than or equal to original" << endl;
    //  return 1;
    // }

    string input = argv[1];
    string output = argv[2];
    int width = stoi(argv[3]);
    int height = stoi(argv[4]);

    // cout << "width: " << width << endl;
    // cout << "height: " << height << endl;

    // cout << "Resizing " << input << " to " << width << "x" << height << " With filename: " << output << endl;

    ifstream fin(input);

    if (!fin) {
        cout << "Error opening file: " << input << endl;
    }

    ofstream fout(output);

    if (!fout) {
        cout << "Error opening file: " << output << endl;
    }

    Image original;
    Image_init(&original, fin);

    if (width > Image_width(&original) || !(width > 0) || height > Image_height(&original) || !(height > 0)) {
        cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
     << "WIDTH and HEIGHT must be less than or equal to original" << endl;
     return 1;
    }

    seam_carve(&original, width, height);

    Image_print(&original, fout);
}