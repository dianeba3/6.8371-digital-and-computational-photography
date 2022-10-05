/* -----------------------------------------------------------------
 * File:    a0.cpp
 * Created: 2015-09-09
 * Modified: 2017-09-05
 * -----------------------------------------------------------------
 *
 * Implementation file for PS00
 *
 * We define and implement the functions we declared in a0.h
 *
 * ---------------------------------------------------------------*/

// This command imports all the content of a0.h, thus declaring our functions.
#include "a0.h"
#include <math.h>       /* ceil */

// We use the namespace "std" so that we can write the command "cout" instead
// of the verbose version "std::cout".
using namespace std;

// Return the sum of two numbers
float getSum(float a, float b) {
  // --------- HANDOUT  PS00 ------------------------------
  // Create a variable c that is the sum of a and b
  // Return it
  float sum = a + b;
  return sum;
}

// Print the sum of two numbers to the screen
void helloworld(float a, float b) {
  // --------- HANDOUT  PS00 ------------------------------
  // Create a variable c that is the sum of a and b
  // Use cout to display the values
  float sum = a + b;

  // prints sum 
  cout << "Hello World!" << endl << "The value of a is " << a << "." << endl 
  << "The value of b is " << b << "." << endl
  << "The sum of a and b is " << sum << "." << endl;  
}

// Create an image and return it
// You can test your function by calling my_image.write(filename) to write the
// output somewhere
Image readAnImage(const std::string &filename) {
  // --------- HANDOUT  PS00 ------------------------------
  // Use the constructor in Image.h
  return Image(filename); // Change this
}

// Create an empty image of size width x height
SimpleImage::SimpleImage(int width, int height) {
    // --------- HANDOUT  PS00 ------------------------------
    // Make sure keep around all necessary data
    // Initialize the image to empty (0.0f) 
    width_ = width;
    height_ = height;

    cout << width_ << " x " << height_ << endl;

    vector<float> img;
    img.assign(width_*height_, 0.0);
    img_ = img;

}

// Computes the average pixel value of the image
float SimpleImage::compute_average() const {
  // --------- HANDOUT  PS00 ------------------------------
  // compute the average
  // return the average
  float result = 0.0;
  for (int i = 0; i < img_.size(); i++){
    result = result + img_[i];
  }
  return result / img_.size(); 
}

// Create a white (1.0f) rectangle in the center of the image, taking up
// 10% of the image in each direction
void SimpleImage::make_rectangle() {
  // --------- HANDOUT  PS00 ------------------------------
  // Denote white as 1.0f
  // Make sure rectangle is centered in the image and includes the center

  float widthPixels = width_ * 0.2;
  float heightPixels = height_ * 0.2;

  float widthFactor = widthPixels / 2.0;
  float heightFactor = heightPixels / 2.0;

  float widthCenter = (width_ - 1.0) / 2.0;
  float heightCenter = (height_  - 1.0) / 2.0;

  float yAxis = round(heightCenter - heightFactor + 0.01);
  float yAxisEnd = round(heightCenter + heightFactor - 0.01);

  cout << heightFactor << endl;
  
  float xAxis = round(widthCenter - widthFactor + 0.01);
  float xAxisEnd = round(widthCenter + widthFactor  - 0.01);

  for (float j = yAxis; j < yAxisEnd + 1; j++){
    for (float i = xAxis; i < xAxisEnd + 1; i++){
      int index = i + j*width_;
      img_[index] = 1.0f;
    }
  }

}

// Print the image to the terminal.
// denote black (0.0f) as '-' and white (1.0) as '+'
void SimpleImage::print_to_terminal() const {
  // --------- HANDOUT  PS00 ------------------------------
  // print the image as ASCII characters

  for (int j = 0; j < height_; j++){
    for (int i = 0; i < width_; i++){
      int index = i + j*width_;
      if (img_[index] == 0.0){
        cout << "- ";
      } else {
        cout << "+ ";
      }
    }
    cout << endl;   
  }
}
