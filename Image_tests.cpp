#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Sets various pixels in a 2x2 Image and checks
// that Image_print produces the correct output.
TEST(test_print_basic) {
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  // Capture our output
  ostringstream s;
  Image_print(&img, s);

  // Correct output
  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

// IMPLEMENT YOUR TEST FUNCTIONS HERE
// You are encouraged to use any functions from Image_test_helpers.hpp as needed.

// Test case for initializing an Image with width and height, and checking pixel values
TEST(test_image_init) {
  Pixel black = {0, 0, 0};
  Image img;
  Image_init(&img, 3, 2);

  ASSERT_EQUAL(Image_width(&img), 3);
  ASSERT_EQUAL(Image_height(&img), 2);

  // Check if all pixels are initialized to black (0, 0, 0)
  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 3; col++) {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, row, col), black));
    }
  }
}

// Test case for setting and getting pixel values
TEST(test_image_set_and_get_pixel) {
  Image img;
  Image_init(&img, 3, 2);
  
  Pixel color = {123, 234, 45};
  Image_set_pixel(&img, 1, 1, color);

  ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, 1, 1), color));
}

// Test case for initializing an Image from a PPM input stream
TEST(test_image_from_stream) {
  Image img;
  string input = "P3\n2 2\n255\n255 0 0 0 255 0\n0 0 255 255 255 255\n";
  std::istringstream ss_input(input);
  
  Image_init(&img, ss_input);

  ASSERT_EQUAL(Image_width(&img), 2);
  ASSERT_EQUAL(Image_height(&img), 2);

  Pixel expected_pixels[2][2] = {
    { {255, 0, 0}, {0, 255, 0} },
    { {0, 0, 255}, {255, 255, 255} }
  };

  // Check if the pixels match the expected values from the input PPM
  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 2; col++) {
      ASSERT_TRUE(Pixel_equal(Image_get_pixel(&img, row, col), expected_pixels[row][col]));
    }
  }
}

// Test case for writing an Image to an output stream in PPM format
TEST(test_image_to_stream) {
  Image img;
  Image_init(&img, 2, 2);

  Pixel pixels[2][2] = {
    { {255, 0, 0}, {0, 255, 0} },
    { {0, 0, 255}, {255, 255, 255} }
  };

  for (int row = 0; row < 2; row++) {
    for (int col = 0; col < 2; col++) {
      Image_set_pixel(&img, row, col, pixels[row][col]);
    }
  }

  std::ostringstream ss_output;
  Image_print(&img, ss_output);

  string output_correct = "P3\n2 2\n255\n255 0 0 0 255 0 \n0 0 255 255 255 255 \n";
  ASSERT_EQUAL(ss_output.str(), output_correct);
}

// Test case for checking if two Images are equal
TEST(test_image_equal) {
  Image img1, img2;
  Image_init(&img1, 3, 2);
  Image_init(&img2, 3, 2);

  // Check equality when images are identical
  ASSERT_TRUE(Image_equal(&img1, &img2));

  // Modify a pixel in img1 and check that the images are no longer equal
  Pixel red = {255, 0, 0};
  Image_set_pixel(&img1, 0, 0, red);
  ASSERT_FALSE(Image_equal(&img1, &img2));

  // Set the same pixel in img2 and check equality again
  Image_set_pixel(&img2, 0, 0, red);
  ASSERT_TRUE(Image_equal(&img1, &img2));
}

TEST_MAIN() // Do NOT put a semicolon here
