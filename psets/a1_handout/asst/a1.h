/* -----------------------------------------------------------------
 * File:    a1.h
 * Created: 2015-09-15
 * -----------------------------------------------------------------
 *
 * Assignment 01
 *
 * ---------------------------------------------------------------*/

#ifndef __A1__H
#define __A1__H

#include "Image.h"
#include <cmath>
#include <iostream>

using namespace std;

// Special image
Image create_special();

// Brightness and contrast
Image brightness(const Image &im, float factor);
Image contrast(const Image &im, float factor, float midpoint = 0.5);

// Colorspace problems
static const float weight_init[3] = {0.299, 0.587, 0.114};
Image color2gray(const Image &im,
                 const std::vector<float> &weights =
                     std::vector<float>(weight_init, weight_init + 3));

std::vector<Image> lumiChromi(const Image &im);

Image brightnessContrastLumi(const Image &im, float brightF, float contrastF,
                             float midpoint = 0.3);

Image rgb2yuv(const Image &im);

Image yuv2rgb(const Image &im);

Image saturate(const Image &im, float k);

Image gamma_code(const Image &im, float gamma);

Image quantize(const Image &im, int bits);

std::vector<Image> gamma_test(const Image &im, int bits, float gamma);

std::vector<Image> spanish(const Image &im);

// White Balance
Image grayworld(const Image &in);

std::vector<float> find_mean_of_channel(const Image &im);

#endif
