#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for(int r = 0; r < height; r++){
    for(int c = 0; c < width; c++){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
}

// ADD YOUR TESTS HERE
// You are encouraged to use any functions from Matrix_test_helpers.hpp as needed.

// Tests the initialization of a Matrix object with given width and height
TEST(test_matrix_init) {
    Matrix mat;
    Matrix_init(&mat, 4, 6);

    ASSERT_EQUAL(Matrix_width(&mat), 4);
    ASSERT_EQUAL(Matrix_height(&mat), 6);

    // Check that all elements are initialized to 0
    for (int r = 0; r < 6; r++) {
        for (int c = 0; c < 4; c++) {
            ASSERT_EQUAL(*Matrix_at(&mat, r, c), 0);
        }
    }
}

// Tests Matrix_fill by filling a matrix with a specific value
TEST(test_matrix_fill) {
    Matrix mat;
    Matrix_init(&mat, 3, 3);
    Matrix_fill(&mat, 5);

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            ASSERT_EQUAL(*Matrix_at(&mat, r, c), 5);
        }
    }
}

// Tests Matrix_fill_border by filling the borders of a matrix with a specific value
TEST(test_matrix_fill_border) {
    Matrix mat;
    Matrix_init(&mat, 4, 4);
    Matrix_fill_border(&mat, 9);

    // Check borders
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == 0 || r == 3 || c == 0 || c == 3) {
                ASSERT_EQUAL(*Matrix_at(&mat, r, c), 9);
            } else {
                ASSERT_EQUAL(*Matrix_at(&mat, r, c), 0);
            }
        }
    }
}

// Tests that Matrix_max returns the maximum element in the matrix
TEST(test_matrix_max) {
    Matrix mat;
    Matrix_init(&mat, 3, 3);
    Matrix_fill(&mat, 5);
    *Matrix_at(&mat, 1, 1) = 10; // Set a specific value

    ASSERT_EQUAL(Matrix_max(&mat), 10);
}

// Tests Matrix_column_of_min_value_in_row for finding the correct column of the minimum value
TEST(test_matrix_column_of_min_value_in_row) {
    Matrix mat;
    Matrix_init(&mat, 4, 3);
    *Matrix_at(&mat, 0, 0) = 10;
    *Matrix_at(&mat, 0, 1) = 5;  // Minimum value
    *Matrix_at(&mat, 0, 2) = 8;
    *Matrix_at(&mat, 0, 3) = 6;

    // cout << Matrix_column_of_min_value_in_row(&mat, 0, 0, 4);

    ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 0, 0, 4), 1);
}

// Tests Matrix_min_value_in_row for finding the minimum value in a specific row region
TEST(test_matrix_min_value_in_row) {
    Matrix mat;
    Matrix_init(&mat, 4, 3);
    *Matrix_at(&mat, 1, 0) = 10;
    *Matrix_at(&mat, 1, 1) = 3;  // Minimum value in the row
    *Matrix_at(&mat, 1, 2) = 8;
    *Matrix_at(&mat, 1, 3) = 6;

    ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 1, 0, 4), 3);
}

// Tests Matrix_width for returning the correct width of the matrix
TEST(test_matrix_width) {
    Matrix mat;
    Matrix_init(&mat, 5, 7);

    ASSERT_EQUAL(Matrix_width(&mat), 5);
}

// Tests Matrix_height for returning the correct height of the matrix
TEST(test_matrix_height) {
    Matrix mat;
    Matrix_init(&mat, 5, 7);

    ASSERT_EQUAL(Matrix_height(&mat), 7);
}

// Tests Matrix_at for returning a modifiable pointer to an element in the matrix
TEST(test_matrix_at_modify) {
    Matrix mat;
    Matrix_init(&mat, 3, 3);
    int* elem = Matrix_at(&mat, 1, 1);

    *elem = 99; // Modify the element
    ASSERT_EQUAL(*Matrix_at(&mat, 1, 1), 99);
}

// Tests const Matrix_at for returning a pointer-to-const to an element in the matrix
TEST(test_matrix_at_const) {
    Matrix mat;
    Matrix_init(&mat, 3, 3);
    *Matrix_at(&mat, 1, 1) = 50;

    const Matrix* const_mat = &mat;
    ASSERT_EQUAL(*Matrix_at(const_mat, 1, 1), 50);
}

// Tests that Matrix_fill correctly handles negative values
TEST(test_matrix_fill_negative_value) {
    Matrix mat;
    Matrix_init(&mat, 3, 3);
    Matrix_fill(&mat, -10);

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            ASSERT_EQUAL(*Matrix_at(&mat, r, c), -10);
        }
    }
}

// Tests that Matrix_fill_border works when the matrix is very small (1x1)
TEST(test_matrix_fill_border_small) {
    Matrix mat;
    Matrix_init(&mat, 1, 1);
    Matrix_fill_border(&mat, 15);

    ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), 15);
}

// Tests that Matrix_column_of_min_value_in_row works when the region has identical values
TEST(test_matrix_column_of_min_value_in_row_identical) {
    Matrix mat;
    Matrix_init(&mat, 4, 3);
    Matrix_fill(&mat, 7);

    ASSERT_EQUAL(Matrix_column_of_min_value_in_row(&mat, 0, 0, 4), 0); // First column should be returned
}

// Tests that Matrix_min_value_in_row works with negative values
TEST(test_matrix_min_value_in_row_negative_values) {
    Matrix mat;
    Matrix_init(&mat, 4, 3);
    *Matrix_at(&mat, 1, 0) = -5;
    *Matrix_at(&mat, 1, 1) = -10; // Minimum value in the row
    *Matrix_at(&mat, 1, 2) = 3;
    *Matrix_at(&mat, 1, 3) = 2;

    ASSERT_EQUAL(Matrix_min_value_in_row(&mat, 1, 0, 4), -10);
}

// Tests that Matrix_max works when the maximum element is at the last position
TEST(test_matrix_max_last_element) {
    Matrix mat;
    Matrix_init(&mat, 3, 3);
    Matrix_fill(&mat, 1);
    *Matrix_at(&mat, 2, 2) = 100; // Set the maximum at the last element

    ASSERT_EQUAL(Matrix_max(&mat), 100);
}

TEST_MAIN() // Do NOT put a semicolon here