/* -----------------------------------------------------------------
 * File:    a3_main.cpp
 * Author:  Michael Gharbi <gharbi@mit.edu>
 * Created: 2015-09-30
 * -----------------------------------------------------------------
 *
 *
 *
 * ---------------------------------------------------------------*/

#include "Image.h"
#include "align.h"
#include "basicImageManipulation.h"
#include "demosaic.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of demosaic.cpp and align.cpp
int main() {
  // cout << "nothing done in a3_main.cpp, debug me !" << endl;
  // Denoise ---------------------------
  // Load sequence
  // vector<Image> seq;
  // int n_images = 16;
  // for (int i = 1; i <= n_images; ++i) {
  //     ostringstream fname;
  //     // fname << "./Input/aligned-ISO400/1D2N-iso400-";
  //     fname << "./Input/aligned-ISO3200/1D2N-iso3200-";
  //     fname << i;
  //     fname << ".png";
  //     seq.push_back(Image(fname.str()));
  // }
  
  // Denoise
  // Image out = logSNR(seq);
  // out.write("./Output/logSNR_iso3200_16_images.png");

  Image im1 = Image(100,100,1);
  Image im2 = Image(100,100,1);

  im1.set_color(0.0, 0.0, 0.0);
  im2.set_color(0.0, 0.0, 0.0);

  im1.create_rectangle(40, 40, 60, 50, 1.0, 1.0, 1.0);
  im2.create_rectangle(50, 60, 70, 70, 1.0, 1.0, 1.0);

  int maxOffset = 20;
  vector<int> offset = align(im1, im2, maxOffset);
  cout << "x offset: " << offset[0] << " y offset: " << offset[1] << endl;

  // vector<Image> seq;
  // int n_images = 18;
  // for (int i = 1; i <= n_images; ++i) {
  //     ostringstream fname;
  //     fname << "./Input/green/noise-small-";
  //     fname << i;
  //     fname << ".png";
  //     seq.push_back(Image(fname.str()));
  // }
  
  // Denoise
  // Image out = alignAndDenoise(seq, 10);
  // Image out2 = denoiseSeq(seq);
  // out.write("./Output/align_and_denoise_green_18_images.png");
  // out2.write("./Output/denoise_green_18_images.png");


  //
  // Image SNRIm = logSNR(seq,1/30.0);
  // SNRIm.write("./Output/snr_map.png");
  //
  // Demosaic ---------------------------
  Image raw("./Input/raw/card.png");
  // Image green = basicGreen(raw, 1);
  // green.write("./Output/demosaic_green.png");
  // Image red = basicRorB(raw, 1, 1);
  // red.write("./Output/demosaic_red.png");
  // Image blue = basicRorB(raw, 0, 0);
  // blue.write("./Output/demosaic_blue.png");

  // Image newIm = basicDemosaic(raw, 1, 1,1,0,0);
  // newIm.write("./Output/cash_basic_demosaiced.png");


  // Image rgb = edgeBasedGreenDemosaic(raw, 1, 1,1,0,0);
  // rgb.write("./Output/cash_EdgeGreen_demosaiced.png");

  Image improved = improvedDemosaic(raw, 1, 1,1,0,0);
  improved.write("./Output/card_Improved_demosaiced.png");
  //
  //
  // // Sergey ---------------------------
  // Image sergeyImg("./Input/Sergey/00911v_third.png");
  // Image rgb2 = split(sergeyImg);
  // rgb2.write("./Output/Sergey_split.png");
  // Image rgbAlign = sergeyRGB(sergeyImg,10);
  // rgbAlign.write("./Output/Sergey_aligned.png");

  return 0;
}
