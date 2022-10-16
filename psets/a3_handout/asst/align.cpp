/* --------------------------------------------------------------------------
 * File:    align.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "align.h"
#include <algorithm>

using namespace std;

Image denoiseSeq(const vector<Image> &imSeq) {
  // // --------- HANDOUT  PS03 ------------------------------
  // Basic denoising by computing the average of a sequence of images
  Image result = Image(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());

  for (int i = 0; i < imSeq.size(); i++){
    result = result + imSeq[i];
  }
  float fSize = imSeq.size();

  result = result / fSize;

  return result;

  //question: the pictures with iso 400 came out a lot clearer and like the noise was completely eliminated
  // and that makes sense because a higher iso means that there will be a lot more noise in the image because higher ISO means more amplification

}

Image logSNR(const vector<Image> &imSeq, float scale) {
  // // --------- HANDOUT  PS03 ------------------------------
  // returns an image visualizing the per-pixel and
  // per-channel log of the signal-to-noise ratio scaled by scale.

  Image sigma = Image(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());
  Image result = Image(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());
  Image meanImage = denoiseSeq(imSeq);

  for (int i = 0; i < imSeq.size(); i++) {
    sigma = sigma + ((imSeq[i] - meanImage)*(imSeq[i] - meanImage));
  }
  sigma = sigma / (imSeq.size() - 1);

  Image meanSquared = Image(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());
  for (int j = 0; j < imSeq.size(); j++){
    meanSquared = meanSquared + (imSeq[j]*imSeq[j]);
  }
  meanSquared = meanSquared / (imSeq.size());

  for (int z = 0; z < result.channels(); z++) {
		for (int y = 0; y < result.height(); y++)  {
			for (int x = 0; x < result.width(); x++)  {
				result(x,y,z)  = 10 * log10(meanSquared(x,y,z) / sigma(x,y,z)) * scale;
			}
		}
	}
  return result;

  //question: the iso400 picture looks a lot brighter in more areas than the ios3200 picture which means it has a better SNR value
}

vector<int> align(const Image &im1, const Image &im2, int maxOffset) {
  // // --------- HANDOUT  PS03 ------------------------------
  // returns the (x,y) offset that best aligns im2 to match im1.

  vector<int> result;

  vector<vector<int>> alignments;
  vector<int> meanSqauredError;
  float squareError;

  vector<int> tempAlignments;
  
  for (int y = -maxOffset; y <= maxOffset; y++) {
    for (int x = -maxOffset; x <= maxOffset; x++)  {
      tempAlignments = {x, y};
      alignments.push_back(tempAlignments);

      Image translatedImage = roll(im2, x, y);
      squareError = 0.0;
      for (int j = maxOffset; j < im1.height() - maxOffset; j++) {
        for (int i = maxOffset; i < im1.width() - maxOffset; i++)  {
          for (int z = 0; z < im1.channels(); z++) {
            squareError += pow((im1.smartAccessor(i,j,z, true)-translatedImage.smartAccessor(i,j,z, true)), 2);
          }
        }
      }
      meanSqauredError.push_back(squareError);
    } 
  }
  
  int index_min = min_element(meanSqauredError.begin(),meanSqauredError.end()) - meanSqauredError.begin();

  return alignments[index_min];
}

Image alignAndDenoise(const vector<Image> &imSeq, int maxOffset) {
  // // --------- HANDOUT  PS03 ------------------------------
  // Registers all images to the first one in a sequence and outputs
  // a denoised image even when the input sequence is not perfectly aligned.
  Image result = Image(imSeq[0].width(), imSeq[0].height(), imSeq[0].channels());

  vector<int> alignments;
  vector<Image> newAlignedImages;
  newAlignedImages.push_back(imSeq[0]);

  for (int i = 1; i < imSeq.size(); i++){
    alignments = align(imSeq[0], imSeq[i], 10);
    newAlignedImages.push_back(roll(imSeq[i], alignments[0], alignments[1])); //translate with x,y
  }

  result = denoiseSeq(newAlignedImages);

  return result;
}

Image split(const Image &sergeyImg) {
  // --------- HANDOUT  PS03 ------------------------------
  // 6.865 only:
  // split a Sergey images to turn it into one 3-channel image.
  return Image(1, 1, 1);
}

Image sergeyRGB(const Image &sergeyImg, int maxOffset) {
  // // --------- HANDOUT  PS03 ------------------------------
  // 6.865 only:
  // aligns the green and blue channels of your rgb channel of a sergey
  // image to the red channel. This should return an aligned RGB image
  return Image(1, 1, 1);
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// This circularly shifts an image by xRoll in the x direction and
// yRoll in the y direction. xRoll and yRoll can be negative or postive
Image roll(const Image &im, int xRoll, int yRoll) {

  int xNew, yNew;
  Image imRoll(im.width(), im.height(), im.channels());

  // for each pixel in the original image find where its corresponding
  // location is in the rolled image
  for (int x = 0; x < im.width(); x++) {
    for (int y = 0; y < im.height(); y++) {

      // use modulo to figure out where the new location is in the
      // rolled image. Then take care of when this returns a negative
      // number
      xNew = (x + xRoll) % im.width();
      yNew = (y + yRoll) % im.height();
      xNew = (xNew < 0) * (imRoll.width() + xNew) + (xNew >= 0) * xNew;
      yNew = (yNew < 0) * (imRoll.height() + yNew) + (yNew >= 0) * yNew;

      // assign the rgb values for each pixel in the original image to
      // the location in the new image
      for (int z = 0; z < im.channels(); z++) {
        imRoll(xNew, yNew, z) = im(x, y, z);
      }
    }
  }

  // return the rolled image
  return imRoll;
}
