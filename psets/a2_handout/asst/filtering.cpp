/* -----------------------------------------------------------------
 * File:    filtering.cpp
 * Created: 2015-09-22
 * -----------------------------------------------------------------
 *
 * Image convolution and filtering
 *
 * ---------------------------------------------------------------*/

#include "filtering.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include "basicImageManipulation.h"
using namespace std;

Image boxBlur(const Image &im, int k, bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Convolve an image with a box filter of size k by k
  // It is safe to asssume k is odd.

  Image result = Image(im.width(), im.height(), im.channels());
  int kernel = k / 2;
  float factor = 1.0 / float(k * k);

  for (int z = 0; z < im.channels(); z++){
    for (int y = 0; y < im.height(); y++) {
      for (int x = 0; x < im.width(); x++) {
        for (int j = -kernel; j <= kernel; j++) {
          for (int i = -kernel; i <= kernel; i++) {
            result(x,y,z) += im.smartAccessor(x+i, y+j, z, clamp) * factor;
          }
        }
      }
    }
  }

  return result; 
}

Image Filter::convolve(const Image &im, bool clamp) const {
  // --------- HANDOUT  PS02 ------------------------------
  // Write a convolution function for the filter class
  Image result = Image(im.width(), im.height(), im.channels());
  int x_kernel = width_ / 2;
  int y_kernel = height_ / 2;

  for (int z = 0; z < im.channels(); z++){
    for (int y = 0; y < im.height(); y++) {
      for (int x = 0; x < im.width(); x++) {
        for (int j = -y_kernel; j <= y_kernel; j++) {
          for (int i = -x_kernel; i <= x_kernel; i++) {
  					result(x,y,z) += operator()(i+x_kernel, j+y_kernel) * im.smartAccessor(x-i, y-j, z, clamp);
          }
        }
      }
    }
  }

  return result; 
}

Image boxBlur_filterClass(const Image &im, int k, bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Reimplement the box filter using the filter class.
  // check that your results match those in the previous function "boxBlur"
  std::vector<float> kernel;
	for (int i = 0; i < k*k; i++) {
		kernel.push_back(1.0/(float(k*k)));
	}
	Filter box_blur(kernel, k, k);
  Image result = box_blur.convolve(im, clamp);

  return result;
}

Image gradientMagnitude(const Image &im, bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Uses a Sobel kernel to compute the horizontal and vertical components
  // of the gradient of an image and returns the gradient magnitude.
  Image result = Image(im.width(), im.height(), im.channels());

  vector<float> fDataX{-1.0, 0.0, 1.0, -2.0, 0.0, 2.0, -1.0, 0.0, 1.0};
  vector<float> fDataY{-1.0, -2.0, -1.0, 0.0, 0.0, 0.0, 1.0, 2.0, 1.0};

  Filter h_sobel(fDataX, 3, 3);
	Filter v_sobel(fDataY, 3, 3);

  Image horizontal_Sobel = h_sobel.convolve(im);
  Image vertical_Sobel = v_sobel.convolve(im);

  for (int z = 0; z < im.channels(); z++) {
    for (int y = 0; y < im.height(); y++) {
      for (int x = 0; x < im.width(); x++) {
        result(x,y,z) = sqrt((horizontal_Sobel(x,y,z)*horizontal_Sobel(x,y,z)) + (vertical_Sobel(x,y,z)*vertical_Sobel(x,y,z)));
      }
    }
  }

  return result; 
}

vector<float> gauss1DFilterValues(float sigma, float truncate) {
  // --------- HANDOUT  PS02 ------------------------------
  // Create a vector containing the normalized values in a 1D Gaussian filter
  // Truncate the gaussian at truncate*sigma.
  std::vector<float> result;

  int neighbors = ceil(sigma * truncate);
	float normalization_factor;
  float gauss_value;
	for (int i = -neighbors; i <=neighbors; i++) {
    gauss_value = exp(-(i*i) / (2*sigma*sigma));
		result.push_back(gauss_value);
		normalization_factor += gauss_value;
	}

	for (auto & gaussVal : result) {
		gaussVal = gaussVal / normalization_factor;
	}
	
	return result;
}

Image gaussianBlur_horizontal(const Image &im, float sigma, float truncate,
                              bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Gaussian blur across the rows of an image

  std::vector<float> kernel = gauss1DFilterValues(sigma, truncate);

	Filter gauss_1d(kernel, 2*ceil(truncate*sigma)+1, 1); //1 in the end for only horizontal

	return gauss_1d.convolve(im, clamp);

}


vector<float> gauss2DFilterValues(float sigma, float truncate) {
  // --------- HANDOUT  PS02 ------------------------------
  // Create a vector containing the normalized values in a 2D Gaussian
  // filter. Truncate the gaussian at truncate*sigma.

  std::vector<float> result;

  int neighbors = ceil(sigma * truncate);
	float normalization_factor;
  float gauss_value;

	for (int y = -neighbors; y <=neighbors; y++) {
    for (int x = -neighbors; x <= neighbors; x++){
      gauss_value = exp(-((x*x)+(y*y)) / (2*sigma*sigma));
      result.push_back(gauss_value);
      normalization_factor += gauss_value;
    } 
	}

	for (auto & gaussVal : result) {
		gaussVal = gaussVal / normalization_factor;
	}
	
	return result;
}

Image gaussianBlur_2D(const Image &im, float sigma, float truncate,
                      bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Blur an image with a full 2D rotationally symmetric Gaussian kernel

  std::vector<float> kernel = gauss2DFilterValues(sigma, truncate);

	Filter gauss_2d(kernel, 2*ceil(truncate*sigma)+1, 2*ceil(truncate*sigma)+1);

	return gauss_2d.convolve(im, clamp);
}

Image gaussianBlur_separable(const Image &im, float sigma, float truncate,
                             bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Use principles of seperabiltity to blur an image using 2 1D Gaussian
  // Filters

  Image horizontal_blur = gaussianBlur_horizontal(im, sigma, truncate, clamp);

  std::vector<float> kernel = gauss1DFilterValues(sigma, truncate);
	Filter vertical_blur(kernel, 1, 2*ceil(truncate*sigma)+1); // 1 in the beginning for vertical blur

	return vertical_blur.convolve(horizontal_blur);

}

Image unsharpMask(const Image &im, float sigma, float truncate, float strength,
                  bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Sharpen an image
  
  Image lowPass = gaussianBlur_2D(im, sigma, truncate, clamp);
  Image subtract_lowPass = strength*(im - lowPass);

  return im + subtract_lowPass;

}

Image bilateral(const Image &im, float sigmaRange, float sigmaDomain,
                float truncateDomain, bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // Denoise an image using the bilateral filter

  Image result = Image(im.width(), im.height(), im.channels());

	std::vector<float> domain_kernel = gauss2DFilterValues(sigmaDomain, truncateDomain);
	Filter domain_filter(domain_kernel, 2*ceil(sigmaDomain * truncateDomain) + 1, 2*ceil(sigmaDomain * truncateDomain) + 1);

	
  int neighbors = ceil(sigmaDomain * truncateDomain);
	for (int z = 0; z < im.channels(); z++) {
		for (int y = 0; y < im.height(); y++) {
			for (int x = 0; x < im.width(); x++) {
        float pixel = im.smartAccessor(x, y, z);

				for (int j = -neighbors; j <= neighbors; j++) {
					for (int i = -neighbors; i <= neighbors; i++) {
            //did not really understand how to do this function and unfortunately did not have time to figure it out
					} 
				}
				result(x,y,z) = 0.0;
			}
		}
	}

	return result;
}

Image bilaYUV(const Image &im, float sigmaRange, float sigmaY, float sigmaUV,
              float truncateDomain, bool clamp) {
  // --------- HANDOUT  PS02 ------------------------------
  // 6.865 only
  // Bilateral Filter an image seperatly for
  // the Y and UV components of an image
  return im;
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// Create an image of 0's with a value of 1 in the middle. This function
// can be used to test that you have properly set the kernel values in your
// Filter object. Make sure to set k to be larger than the size of your kernel
Image impulseImg(int k) {
  // initlize a kxkx1 image of all 0's
  Image impulse(k, k, 1);

  // set the center pixel to have intensity 1
  int center = floor(k / 2);
  impulse(center, center, 0) = 1.0f;

  return impulse;
}

// ------------- FILTER CLASS -----------------------
Filter::Filter() : width_(0), height_(0) {}
Filter::Filter(const vector<float> &fData, int fWidth, int fHeight)
    : kernel_(fData), width_(fWidth), height_(fHeight) {
  assert(fWidth * fHeight == (int)fData.size());
}

Filter::Filter(int fWidth, int fHeight)
    : kernel_(std::vector<float>(fWidth * fHeight, 0)), width_(fWidth),
      height_(fHeight) {}

const float &Filter::operator()(int x, int y) const {
  if (x < 0 || x >= width_)
    throw OutOfBoundsException();
  if (y < 0 || y >= height_)
    throw OutOfBoundsException();

  return kernel_[x + y * width_];
}

float &Filter::operator()(int x, int y) {
  if (x < 0 || x >= width_)
    throw OutOfBoundsException();
  if (y < 0 || y >= height_)
    throw OutOfBoundsException();

  return kernel_[x + y * width_];
}
// --------- END FILTER CLASS -----------------------
