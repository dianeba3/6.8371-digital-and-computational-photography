/* -----------------------------------------------------------------
 * File:    a1.cpp
 * Created: 2015-09-15
 * Updated: 2019-08-10
 * -----------------------------------------------------------------
 *
 * Assignment 01
 *
 * ---------------------------------------------------------------*/

#include "a1.h"
#include <cmath>
using namespace std;

// Create a surprise image
Image create_special() {
  // // --------- HANDOUT  PS01 ------------------------------
  // create the image outlined in the handout
  Image result = Image(290, 150, 3);

  result.set_color(1.0, 1.0, 1.0);
  result.create_rectangle(0, 0, 31, 149, 0.64, 0.12, 0.20);
  result.create_rectangle(52, 0, 83, 102, 0.64, 0.12, 0.20);
  result.create_rectangle(103, 0, 134, 149, 0.64, 0.12, 0.20);
  result.create_rectangle(155, 0, 186, 30, 0.64, 0.12, 0.20);
  result.create_rectangle(155, 48, 186, 149, 0.55, 0.55, 0.55);
  result.create_rectangle(207, 0, 289, 30, 0.64, 0.12, 0.20);
  result.create_rectangle(207, 48, 238, 149, 0.64, 0.12, 0.20);

  return result; 
}

// Change the brightness of the image
// const Image & means a reference to im will get passed to the function,
// but the compiler won't let you modify it within the function.
// So you will return a new image
Image brightness(const Image &im, float factor) {
  // --------- HANDOUT  PS01 ------------------------------
  // Image output(im.width(), im.height(), im.channels());
  // Modify image brightness
  // return output;
  Image result = Image(im.width(), im.height(), im.channels());

  for (int z = 0; z < im.channels(); z++) {
    for (int y = 0; y < im.height(); y++) {
      for (int x = 0; x < im.width(); x++) {
        result(x, y, z) = im(x, y, z) * factor;
      }
    }
  }

  return result; 
}

Image contrast(const Image &im, float factor, float midpoint) {
  // --------- HANDOUT  PS01 ------------------------------
  // Image output(im.width(), im.height(), im.channels());
  // Modify image contrast
  // return output;
  Image result = Image(im.width(), im.height(), im.channels());

  for (int z = 0; z < im.channels(); z++) {
    for (int y = 0; y < im.height(); y++) {
      for (int x = 0; x < im.width(); x++) {
        result(x, y, z) = factor * (im(x, y, z) - midpoint) + midpoint;
      }
    }
  }
  
  return result;
}

Image color2gray(const Image &im, const std::vector<float> &weights) {
  // --------- HANDOUT  PS01 ------------------------------
  // Image output(im.width(), im.height(), 1);
  // Convert to grayscale
  Image result = Image(im.width(), im.height(), 1);

  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      result(x, y) = im(x, y, 0) * weights[0] + im(x, y, 1) * weights[1] +  im(x, y, 2) * weights[2];
    }
  }

  return result;
}

// For this function, we want two outputs, a single channel luminance image
// and a three channel chrominance image. Return them in a vector with
// luminance first
std::vector<Image> lumiChromi(const Image &im) {
  // --------- HANDOUT  PS01 ------------------------------
  // Create the luminance image
  // Create the chrominance image
  // Create the output vector as (luminance, chrominance)

  std::vector<Image> result;

  Image luminance = color2gray(im);
  Image chrominance = Image(im.width(), im.height(), im.channels());
  // You can compute this chrominance by dividing the input image by the luminance
  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      chrominance(x, y, 0) = im(x, y, 0) / luminance(x, y);
      chrominance(x, y, 1) = im(x, y, 1) / luminance(x, y);
      chrominance(x, y, 2) = im(x, y, 2) / luminance(x, y);
    }
  }
  result.push_back(luminance);
  result.push_back(chrominance);

  return result;
}

// Modify brightness then contrast
Image brightnessContrastLumi(const Image &im, float brightF, float contrastF,
                             float midpoint) {
  // --------- HANDOUT  PS01 ------------------------------
  // Modify brightness, then contrast of luminance image
  Image result = Image(im.width(), im.height(), im.channels());

  std::vector<Image> lumiChromiVector = lumiChromi(im);
  
  Image luminance = lumiChromiVector[0];
  luminance = brightness(luminance, brightF); //first bridghtness
  luminance = contrast(luminance, contrastF, midpoint); // second contrast

  //create new image by: Recombine the modified luminance with the 
  //                     chrominance by multiplying (lumi*chromi) to produce the output image.
  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      result(x, y, 0) = lumiChromiVector[1](x, y, 0) * luminance(x, y);
      result(x, y, 1) = lumiChromiVector[1](x, y, 1) * luminance(x, y);
      result(x, y, 2) = lumiChromiVector[1](x, y, 2) * luminance(x, y);
    }
  }

  return result;

}

Image rgb2yuv(const Image &im) {
  // --------- HANDOUT  PS01 ------------------------------
  // Create output image of appropriate size
  // Change colorspace
  Image result = Image(im.width(), im.height(), im.channels());

  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      // An RGB image can be converted to and from a YUV image using the matrix multiplications
      result(x, y, 0) = 0.299*im(x,y,0)+ 0.587*im(x,y,1) + 0.114*im(x,y,2);
      result(x, y, 1) = -0.147*im(x,y,0) + -0.289*im(x,y,1) + 0.436*im(x,y,2);
      result(x, y, 2) = 0.615*im(x,y,0) + -0.515*im(x,y,1) + -0.100*im(x,y,2);
    }
  }

  return result;

}

Image yuv2rgb(const Image &im) {
  // --------- HANDOUT  PS01 ------------------------------
  // Create output image of appropriate size
  // Change colorspace
  Image result = Image(im.width(), im.height(), im.channels());

  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      // A YUV image can be converted to and from a RGB image using the matrix multiplications
      result(x, y, 0) = 1*im(x,y,0)+ 0*im(x,y,1) + 1.14*im(x,y,2);
      result(x, y, 1) = 1*im(x,y,0) + -0.395*im(x,y,1) + -0.581*im(x,y,2);
      result(x, y, 2) = 1*im(x,y,0) + 2.032*im(x,y,1) + 0*im(x,y,2);
    }
  }

  return result;
}

Image saturate(const Image &im, float factor) {
  // --------- HANDOUT  PS01 ------------------------------
  // Create output image of appropriate size
  // Saturate image
  // return output;
  Image yuvImage = rgb2yuv(im);

  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      yuvImage(x, y, 1) *= factor; //U channel
      yuvImage(x, y, 2) *= factor; //V channel
    }
  }

  Image result = yuv2rgb(yuvImage);

  return result;
}

// Gamma codes the image
Image gamma_code(const Image &im, float gamma) {
  // // --------- HANDOUT  PS01 ------------------------------
  // Image output(im.width(), im.height(), im.channels());
  // Gamma encodes the image
  // return output;
  Image result = Image(im.width(), im.height(), im.channels());

  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      result(x, y, 0) = powf(im(x,y,0), 1/gamma);
      result(x, y, 1) = powf(im(x,y,1), 1/gamma);
      result(x, y, 2) = powf(im(x,y,2), 1/gamma);
    }
  }

  return result;
}

// Quantizes the image to 2^bits levels and scales back to 0~1
Image quantize(const Image &im, int bits) {
  // // --------- HANDOUT  PS01 ------------------------------
  // Image output(im.width(), im.height(), im.channels());
  // Quantizes the image to 2^bits levels
  // return output;
  Image result = Image(im.width(), im.height(), im.channels());

  for (int y = 0; y < im.height(); y++) {
    for (int x = 0; x < im.width(); x++) {
      result(x, y, 0) = ((int)(im(x,y,0)*pow(2,bits))) / (powf(2,bits)-1);
      result(x, y, 1) = ((int)(im(x,y,1)*pow(2,bits))) / (powf(2,bits)-1);
      result(x, y, 2) = ((int)(im(x,y,2)*pow(2,bits))) / (powf(2,bits)-1);
    }
  }

  return result;
}

// Compare between first quantize then gamma_encode and first gamma_encode
// then quantize
std::vector<Image> gamma_test(const Image &im, int bits, float gamma) {
  // // --------- HANDOUT  PS01 ------------------------------
  // // im1 = quantize then gamma_encode the image
  // // im2 = gamma_encode then quantize the image
  // // Remember to create the output images and the output vector
  // // Push the images onto the vector
  // // Do all the required processing
  // // Return the vector, color image first

  std::vector<Image> result;

  Image im1 = quantize(im, bits);

  Image im2 = gamma_code(quantize(gamma_code(im, 1/gamma), bits), gamma);

  result.push_back(im1);
  result.push_back(im2);

  return result; 
}

// Return two images in a C++ vector
std::vector<Image> spanish(const Image &im) {
  // --------- HANDOUT  PS01 ------------------------------
  // Remember to create the output images and the output vector
  // Push the images onto the vector
  // Do all the required processing
  // Return the vector, color image first
  std::vector<Image> result;
  Image im2(im.width(), im.height(), im.channels());

  Image yuvImage = rgb2yuv(im);
  for (int y = 0; y < yuvImage.height(); y++) {
    for (int x = 0; x < yuvImage.width(); x++) {
      im2(x, y, 0) = 0.5;
      im2(x, y, 1) = -yuvImage(x, y, 1); //-U
      im2(x, y, 2) = -yuvImage(x, y, 2); //-V
    }
  }

  im2 = yuv2rgb(im2);
  Image im1 = color2gray(im);

  for (int i = 0; i < im1.channels(); i++){
    im1(floor(im.width()/2), floor(im.height()/2), i) = 0;
  }

  for (int i = 0; i < im2.channels(); i++){
    im2(floor(im.width()/2), floor(im.height()/2), i) = 0;
  }


  result.push_back(im2);
  result.push_back(im1);

  return result;
}

// White balances an image using the gray world assumption
Image grayworld(const Image &im) {
  // --------- HANDOUT  PS01 ------------------------------
  // Implement automatic white balance by multiplying each channel
  // of the input by a factor such that the three channel of the output
  // image have the same mean value. The mean value of the green channel
  // is taken as reference.
  Image result(im.width(), im.height(), im.channels());
  std::vector<float> averageChannel = find_mean_of_channel(im);

  float desired = averageChannel[1];
  float factor = 0.0;

  for (int z = 0; z < im.channels(); z++){
    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
          factor = desired / averageChannel[z];
          result(x,y,z) = im(x,y,z) * factor;
        }
      }
  }
  
  return result;
}


std::vector<float> find_mean_of_channel(const Image &im) {
  // --------- HANDOUT  PS01 ------------------------------
  // Implement automatic white balance by multiplying each channel
  // of the input by a factor such that the three channel of the output
  // image have the same mean value. The mean value of the green channel
  // is taken as reference.
  std::vector<float> result(im.channels());

  for (int z = 0; z < im.channels(); z++){
    for (int y = 0; y < im.height(); y++) {
        for (int x = 0; x < im.width(); x++) {
          result[z] = result[z] + im(x,y,z); 
        }
      }
  }


  for (int z = 0; z < im.channels(); z++){
    result[z] = result[z] / (im.width()*im.height());
  }

  return result;
}
