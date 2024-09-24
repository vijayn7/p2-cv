#include <cassert>
#include <vector>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image *img)
{

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r)
  {
    for (int c = 0; c < width; ++c)
    {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image *img)
{

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r)
  {
    for (int c = 0; c < width; ++c)
    {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2)
{
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr * dr + dg * dg + db * db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// ------------------------------------------------------------------
// You may change code below this line!

// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image *img, Matrix *energy)
{
  Matrix_init(energy, Image_width(img), Image_height(img));
  for (int row = 1; row < Image_height(img) - 1; row++)
  { // Corrected loop bounds
    for (int col = 1; col < Image_width(img) - 1; col++)
    { // Corrected loop bounds
      Pixel north = Image_get_pixel(img, row - 1, col);
      Pixel east = Image_get_pixel(img, row, col + 1);
      Pixel south = Image_get_pixel(img, row + 1, col);
      Pixel west = Image_get_pixel(img, row, col - 1);
      int computedEnergy = squared_difference(north, south)
      + squared_difference(west, east);
      *Matrix_at(energy, row, col) = computedEnergy;
    }
  }
  Matrix_fill_border(energy, Matrix_max(energy));
}

// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix *energy, Matrix *cost)
{
  Matrix_init(cost, Matrix_width(energy), Matrix_height(energy));

  for (int col = 0; col < Matrix_width(cost); col++)
  {
    *Matrix_at(cost, 0, col) = *Matrix_at(energy, 0, col);
  }

  for (int row = 1; row < Matrix_height(cost); row++)
  {
    for (int col = 0; col < Matrix_width(cost); col++)
    {
      int minCost;
      if (col < 1)
      {
        minCost = *Matrix_at(energy, row, col) +
        min(*Matrix_at(cost, row - 1, col),
        *Matrix_at(cost, row - 1, col + 1));
      }
      else if (col >= Matrix_width(cost) - 1)
      {
        minCost = *Matrix_at(energy, row, col) +
        min(*Matrix_at(cost, row - 1, col),
        *Matrix_at(cost, row - 1, col - 1));
      }
      else
      {
        minCost = *Matrix_at(energy, row, col) +
        min(*Matrix_at(cost, row - 1, col),
        min(*Matrix_at(cost, row - 1, col - 1),
        *Matrix_at(cost, row - 1, col + 1)));
      }
      *Matrix_at(cost, row, col) = minCost;
    }
  }
}

// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the column numbers for
//           each pixel along the seam, starting with the lowest numbered row (top
//           of image) and progressing to the highest (bottom of image). The length
//           of the returned vector is equal to Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest column number) is used.
//           See the project spec for details on computing the minimal seam.
vector<int> find_minimal_vertical_seam(const Matrix *cost)
{
  vector<int> seam = vector<int>(Matrix_height(cost));

  // Matrix_print(cost, cout);

  seam[seam.size() - 1] = 
  Matrix_column_of_min_value_in_row(
    cost, Matrix_height(cost) - 1, 0, Matrix_width(cost) - 1);

  for (int i = seam.size() - 2; i >= 0; i--)
  {
    int col = seam[i + 1];
    if (col == 0)
    {
      seam[i] = Matrix_column_of_min_value_in_row(cost, i, col, col + 2);
    }
    else if (col == Matrix_width(cost) - 1)
    {
      seam[i] = Matrix_column_of_min_value_in_row(cost, i, col - 1, col + 1);
    }
    else
    {
      seam[i] = Matrix_column_of_min_value_in_row(cost, i, col - 1, col + 2);
    }
  }

  return seam;
}

// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam)
{
  Image smaller;
  Image_init(&smaller, Image_width(img) - 1, Image_height(img));

  for (int row = 0; row < Image_height(img); row++)
  {
    for (int col = 0; col < Image_width(img); col++)
    {
      if (col == seam[row])
      {
        continue;
      }
      Image_set_pixel(&smaller, row, col - (col > seam[row]),
      Image_get_pixel(img, row, col));
    }
  }

  *img = smaller;
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use a vector to hold the seam, and make sure that it has
//           the right size. You can use .data() on a vector to get
//           the underlying array.
void seam_carve_width(Image *img, int newWidth)
{
  while (Image_width(img) > newWidth)
  {
    Matrix energy;
    compute_energy_matrix(img, &energy);
    Matrix cost;
    compute_vertical_cost_matrix(&energy, &cost);
    vector<int> seam = find_minimal_vertical_seam(&cost);
    remove_vertical_seam(img, seam);
  }
}

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight)
{
  rotate_left(img);
  seam_carve_width(img, newHeight);
  rotate_right(img);
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight)
{
  seam_carve_width(img, newWidth);
  seam_carve_height(img, newHeight);
}
