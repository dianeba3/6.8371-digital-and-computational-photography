#include "a1.h"
#include <iostream>

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of a1.cpp and Image.cpp
int main() {

  // Example
  // Image im(5,5,3);
  // cout << im.number_of_elements() << endl;
  // im.set_color(1,2,3);
  // im.create_rectangle(1,1,3,3,1,2,3);
  // im.print_to_terminal();

  // Image im = create_special();
  // Image brighter = brightness(im, 2.0);
  // brighter.write("./brighter_image_2.png");
  // std::vector<Image> LC = lumiChromi(im);
  // LC[0].write("./Output/castle_luminance.png");
  // LC[1].write("./Output/castle_chrominance.png");

  Image underexposed = Image("Input/Boston_underexposed.png");
  Image brighter = brightness(underexposed, 2.0);
  brighter.write("Output/brighter_boston.png");

  Image lowContrast = Image("Input/Boston_low_contrast.png");
  Image higherContrast = contrast(lowContrast, 2.0, 0.7);
  higherContrast.write("Output/contrast_boston.png");

  Image twoImages = Image("Input/castle_small.png");
  std::vector<Image> result1 = lumiChromi(twoImages);
  result1[0].write("Output/lumiCastle.png");
  result1[1].write("Output/chromiCastle.png");

  Image recombine = Image("Input/castle_small.png");
  Image result2 = brightnessContrastLumi(recombine, 1, 2, 0.3);
  result2.write("Output/recombineLumiChromiCastle.png");

  Image rgbCastle = Image("Input/castle_small.png");
  Image result3 = rgb2yuv(rgbCastle);
  result3.write("Output/rgb2yuvCastle.png");

  Image newResult = yuv2rgb(result3);
  newResult.write("Output/yuv2rgbCastle.png");

  Image rgbCastle1 = Image("Input/castle_small.png");
  Image result4 = saturate(rgbCastle1, 2.0);
  // result4.print_to_terminal();
  result4.write("Output/saturateCastle.png");

  Image gammeCastle = Image("Input/castle_small.png");
  Image result5 = gamma_code(gammeCastle, 2.0);
  result5.write("Output/gamme_code_castle.png");  

  // cout << ((int)(0.75*pow(2,2))) / (powf(2,2)-1) << endl;

  Image quant = Image("Input/castle_small.png");
  Image result6 = gamma_code(quant, 2);
  result6.write("Output/quant_castle.png");  

  Image twoImages2 = Image("Input/skies_and_trees.png"); 
  std::vector<Image> result7 = gamma_test(twoImages2, 6, 2.2);
  result7[0].write("Output/im1_sky.png");
  result7[1].write("Output/im2_sky.png");

  Image twoImages3 = Image("Input/castle_small.png"); 
  std::vector<Image> result8 = spanish(twoImages3);
  result8[0].write("Output/spanish_1.png");
  result8[1].write("Output/spanish_2.png");

  Image flower = Image("Input/flower.png"); 
  Image result9 = grayworld(flower);
  result9.write("Output/flower_grayworld.png");


}
