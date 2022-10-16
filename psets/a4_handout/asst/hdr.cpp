// hdr.cpp
// Assignment 5

#include "hdr.h"
#include "filtering.h"
#include <algorithm>
#include <math.h>

using namespace std;

/**************************************************************
 //                       HDR MERGING                        //
 *************************************************************/

Image computeWeight(const Image &im, float epsilonMini, float epsilonMaxi) {
  // --------- HANDOUT  PS04 ------------------------------
  // Generate a weight image that indicates which pixels are good to use in
  // HDR, i.e. weight=1 when the pixel value is in [epsilonMini, epsilonMaxi].
  // The weight is per pixel, per channel.
  Image result = Image(im.width(), im.height(), im.channels());

  for (int z = 0; z < im.channels(); z++) {
		for (int y = 0; y < im.height(); y++)  {
			for (int x = 0; x < im.width(); x++)  {
				if (im(x,y,z) >= epsilonMini and im(x,y,z) <= epsilonMaxi){
          result(x,y,z) = 1.0;
        }else{
          result(x,y,z) = 0.0;
        }
			}
		}
	}

  return result;
}

float computeFactor(const Image &im1, const Image &w1, const Image &im2,
                    const Image &w2) {
  // --------- HANDOUT  PS04 ------------------------------
  // Compute the multiplication factor between a pair of images. This
  // gives us the relative exposure between im1 and im2. It is computed as
  // the median of im2/(im1+eps) for some small eps, taking into account
  // pixels that are valid in both images.
  
  float result = 0.0;
  Image im1_eps = im1 + 1e-10;
  vector<float> im1_usable;
	vector<float> im2_usable;

  for (int z = 0; z < im1.channels(); z++) {
		for (int y = 0; y < im1.height(); y++)  {
			for (int x = 0; x < im1.width(); x++)  {
				if (w1(x,y,z) == 1.0 and w2(x,y,z) == 1.0){ //both pixels are usable so add them to the vectors
          im1_usable.push_back(im1_eps(x,y,z));
          im2_usable.push_back(im2(x,y,z));
        }
			}
		}
	}
  // find median index
  int median = im1_usable.size() / 2;
  // cout << median << endl;
  sort(im1_usable.begin(), im1_usable.end());
  sort(im2_usable.begin(), im2_usable.end());
  //once vectors are sorted in increasing order return factor

  return im2_usable[median]/ im1_usable[median];
}

Image makeHDR(vector<Image> &imSeq, float epsilonMini, float epsilonMaxi) {
  // --------- HANDOUT  PS04 ------------------------------
  // Merge images to make a single hdr image
  // For each image in the sequence, compute the weight map (special cases
  // for the first and last images).
  // Compute the exposure factor for each consecutive pair of image.
  // Write the valid pixel to your hdr output, taking care of rescaling them
  // properly using the factor.
  Image result = Image(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());

  // 1) compute weights for each image
  vector<Image> im_weights;
  //special cases: don’t eliminate dark pixels in the brightest image or bright pixels in the darkest one. 
  // darkest image - don't eliminate bright pixels in the darkest one. 
  im_weights.push_back(computeWeight(imSeq[0], epsilonMini, imSeq[0].max()));

  for (int i = 1; i < imSeq.size()-1; i++){
    im_weights.push_back(computeWeight(imSeq[i], epsilonMini, epsilonMaxi));
  }

  int endIm = imSeq.size() - 1;
  //special case: don’t eliminate dark pixels in the brightest image
  im_weights.push_back(computeWeight(imSeq[endIm], imSeq[endIm].min(), epsilonMaxi));


  // 2) compute factors for each image to compute its multiplicative scale factor
  vector<float> mult_factors;
  mult_factors.push_back(1.0); //image 1 - darkest image
  // cout << mult_factors[0] <<endl;
  for (int i = 1; i < imSeq.size(); i++){
    float new_factor = mult_factors[i-1] * computeFactor(imSeq[i - 1], im_weights[i - 1], imSeq[i], im_weights[i]);
    // cout << new_factor << endl;
    mult_factors.push_back(new_factor);
  }

  // 3) Reconstruct full image using weighted combination per pixel
  for (int z = 0; z < imSeq[0].channels(); z++) {
		for (int y = 0; y < imSeq[0].height(); y++)  {
			for (int x = 0; x < imSeq[0].width(); x++)  {
        float new_val = 0.0;
        float weight_sum = 0.0;
        for (int im = 0; im < imSeq.size(); im++) {
          float weight = im_weights[im](x,y,z);
          weight_sum += weight;
          new_val += weight * (imSeq[im](x,y,z) / mult_factors[im]);
        } 
        // if pixel not assigned then assign it the corresponding value from the first image
        new_val = weight_sum > 0 ? new_val/weight_sum : imSeq[0](x,y,z);
        result(x,y,z) = new_val;
			}
		}
	}

  return result;
}

/**************************************************************
 //                      TONE MAPPING                        //
 *************************************************************/

Image toneMap(const Image &im, float targetBase, float detailAmp, bool useBila,
              float sigmaRange) {
  // --------- HANDOUT  PS04 ------------------------------
  // tone map an hdr image
  // - Split the image into its luminance-chrominance components.
  // - Work in the log10 domain for the luminance
  // -
  Image result = Image(im.width(), im.height(), im.channels());

  // 3.a
  std::vector<Image> lumiChromi_vec = __BASICIMAGEMANIPULATION__H::lumiChromi(im);
  // 3.b
  Image log10_Image = log10Image(lumiChromi_vec[0]);

  // 3.c
  Image base_luminance = Image(im.width(), im.height(), im.channels());
  float sigma = max(im.width(), im.height()) / 50.0;
  if (useBila) {
    base_luminance = __FILTERING__H::bilateral(log10_Image, sigmaRange, sigma);
  }else{
    base_luminance =  __FILTERING__H::gaussianBlur_separable(log10_Image, sigma);
  }

  // 3.d
  Image detail_luminance = log10_Image - base_luminance;

  // 3.e question: differences in bilateral vs. gaussian
  // 

  // 3.f
  float dynamic_range = base_luminance.max() - base_luminance.min();
  float k = log10(targetBase) / dynamic_range;
  Image log10_contrastReduc = detailAmp * detail_luminance + k*(base_luminance - base_luminance.max());

  // 3.g
  Image output = exp10Image(log10_contrastReduc);
  
  std::vector<Image> lumiChromi_2_rgb;
  lumiChromi_2_rgb.push_back(output);
  lumiChromi_2_rgb.push_back(lumiChromi_vec[1]);

  result = lumiChromi2rgb(lumiChromi_2_rgb);

  return result;
}

/*********************************************************************
 *                       Tone mapping helpers                        *
 *********************************************************************/

// image --> log10Image
Image log10Image(const Image &im) {
  // --------- HANDOUT  PS04 ------------------------------
  // Taking a linear image im, transform to log10 scale.
  // To avoid infinity issues, make any 0-valued pixel be equal the the
  // minimum non-zero value. See image_minnonzero(im).
  Image result = Image(im.width(), im.height(), im.channels());

  float minnonzero = image_minnonzero(im);

  for (int z = 0; z < im.channels(); z++) {
		for (int y = 0; y < im.height(); y++)  {
			for (int x = 0; x < im.width(); x++)  {
        if (im(x,y,z) == 0.0) {
          result(x,y,z) =  log10(minnonzero);
        } else {
          result(x,y,z) = log10(im(x,y,z));
        }
			}
		}
	}

  return result;
}

// Image --> 10^Image
Image exp10Image(const Image &im) {
  // --------- HANDOUT  PS04 ------------------------------
  // take an image in log10 domain and transform it back to linear domain.
  // see pow(a, b)
  Image result = Image(im.width(), im.height(), im.channels());

  for (int z = 0; z < im.channels(); z++) {
		for (int y = 0; y < im.height(); y++)  {
			for (int x = 0; x < im.width(); x++)  {
        result(x,y,z) = pow(10, im(x,y,z));
			}
		}
	}

  return result;
}

// min non-zero pixel value of image
float image_minnonzero(const Image &im) {
  // --------- HANDOUT  PS04 ------------------------------
  // return the smallest value in the image that is non-zeros (across all
  // channels too)
  float result = im.max();

  for (int z = 0; z < im.channels(); z++) {
		for (int y = 0; y < im.height(); y++)  {
			for (int x = 0; x < im.width(); x++)  {
        if (im(x,y,z) != 0 and im(x,y,z) < result){
          result = im(x,y,z);
        }
			}
		}
	}
  return result;
}
