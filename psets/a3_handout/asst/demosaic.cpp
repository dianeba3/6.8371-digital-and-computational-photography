/* --------------------------------------------------------------------------
 * File:    demosaic.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "demosaic.h"
#include <cmath>

using namespace std;

//question: because the human eye is more sensitive to green - it produces a better color image

Image basicGreen(const Image &raw, int offset) {
  // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns a single-channel
  // 2D image corresponding to the green channel using simple interpolation

  Image result = Image(raw.width(), raw.height(), 1);

  bool first = (offset == 0) ? true : false;

  for (int y = 1; y < raw.height()-1; y++) { //ignore edges
		for (int x = 1; x < raw.width()-1; x++)  {
			if (first and ((y % 2 == 1 and x % 2 == 1) or (y % 2 == 0 and x % 2 == 0))){ //means its a green pixel
        result(x,y) = raw(x,y);
      }else if (!first and ((y % 2 == 0 and x % 2 == 1) or (y % 2 == 1 and x % 2 == 0))){
        result(x,y) = raw(x,y);
      }else{
        result(x,y) = (raw(x-1,y) + raw(x+1,y) + raw(x,y-1) + raw(x, y+1))/4.0;
      }
		}
	}

  return result;
}

Image basicRorB(const Image &raw, int offsetX, int offsetY) {
  // --------- HANDOUT  PS03 ------------------------------
  //  Takes as input a raw image and returns a single-channel
  //  2D image corresponding to the red or blue channel using simple
  //  interpolation
  Image result = Image(raw.width(), raw.height(), 1);
  
  // bool red = (offsetX == 0 && offsetY == 0) ? true : false;

  // for (int y = 1; y < raw.height()-1; y++) { //ignore edges
	// 	for (int x = 1; x < raw.width()-1; x++)  {
  //     if (red) {
  //       if (y % 2 == 0 and x % 2 == 0){ //reds are both even
  //         result(x,y) = raw(x,y);
  //       }else if (x % 2 == 0 and y % 2 == 1){ //known is left/right
  //         result(x,y) = (raw(x-1,y) + raw(x+1,y))/2.0;
  //       }else if (x % 2 == 1 and y % 2 == 0) { //known is up/down
  //         result(x,y) = (raw(x,y-1) + raw(x,y+1))/2.0;
  //       }else{
  //         result(x,y) = (raw(x-1,y-1) + raw(x-1,y+1) + raw(x+1,y-1) + raw(x+1, y+1))/4.0;
  //       }
  //     }
      // }else{ //blue
      //   if (y % 2 == 1 and x % 2 == 1){ //blues are both odd
      //     result(x,y) = raw(x,y);
      //   }else if (x % 2 == 0 and y % 2 == 1){ //known is left/right
      //     result(x,y) = (raw(x-1,y) + raw(x+1,y))/2.0;
      //   }else if (x % 2 == 1 and y % 2 == 0) { //known is up/down
      //     result(x,y) = (raw(x,y-1) + raw(x,y+1))/2.0;
      //   }else{
      //     result(x,y) = (raw(x-1,y-1) + raw(x-1,y+1) + raw(x+1,y-1) + raw(x+1, y+1))/4.0;
      //   }
      
		// }
	// }

  for (int y = 1; y < raw.height()-1; y++) { //ignore edges
		for (int x = 1; x < raw.width()-1; x++)  {
      if (x % 2 == offsetX and y % 2 == offsetY) { //known value
        result(x,y) = raw(x,y);
      }else if (x % 2 == offsetX and y % 2 != offsetY){ //known is up/down
        result(x,y) = (raw(x,y-1) + raw(x,y+1))/2.0;
      }else if (x % 2 != offsetX and y % 2 == offsetY){ //known is left/right
        result(x,y) = (raw(x-1,y) + raw(x+1,y))/2.0; 
      }else{
        result(x,y) = (raw(x-1,y-1) + raw(x-1,y+1) + raw(x+1,y-1) + raw(x+1,y+1))/4.0; 
      }
		}
	}
  return result;
}

Image basicDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                    int offsetRedY, int offsetBlueX, int offsetBlueY) {
  // --------- HANDOUT  PS03 ------------------------------
  // takes as input a raw image and returns an rgb image
  // using simple interpolation to demosaic each of the channels

  Image result = Image(raw.width(), raw.height(), 3);

  Image green = basicGreen(raw, offsetGreen);
  Image blue = basicRorB(raw, offsetBlueX, offsetBlueY);
  Image red = basicRorB(raw, offsetRedX, offsetRedY);

  // RGB
  for (int y = 0; y < raw.height(); y++) { 
		for (int x = 0; x < raw.width(); x++)  {
      if ((x == 0 or x == raw.width()-1) and (y == 0 or y == raw.height()-1)){
        result(x,y,0) = raw(x,y);
        result(x,y,1) = raw(x,y);
        result(x,y,2) = raw(x,y);

      }
      result(x,y,0) = red(x,y);
      result(x,y,1) = green(x,y);
      result(x,y,2) = blue(x,y);
		}
	}

  return result;
}

Image edgeBasedGreen(const Image &raw, int offset) {
  // --------- HANDOUT  PS03 ------------------------------
  // Takes a raw image and outputs a single-channel
  // image corresponding to the green channel taking into account edges

  //question: we should interpolate along the smallest different direction for a smoother edge

  Image result = Image(raw.width(), raw.height(), 1);

  bool first = (offset == 0) ? true : false;

  for (int y = 1; y < raw.height()-1; y++) { //ignore edges
		for (int x = 1; x < raw.width()-1; x++)  {
			if (first and ((y % 2 == 1 and x % 2 == 1) or (y % 2 == 0 and x % 2 == 0))){ //means its a green pixel
        result(x,y) = raw(x,y);
      }else if (!first and ((y % 2 == 0 and x % 2 == 1) or (y % 2 == 1 and x % 2 == 0))) {
        // cout << "hello" << endl;
        result(x,y) = raw(x,y);
      }else{
        float leftRight = abs(raw(x-1,y) - raw(x+1,y));
        float upDown = abs(raw(x,y-1) - raw(x,y+1));
        if (leftRight > upDown) { //leftRight is bigger - need to take from upDown
          result(x,y) = (raw(x,y-1) + raw(x,y+1))/2.0;
        }else if (upDown >= leftRight){ //upDown is bigger - need to take from leftRight
          result(x,y) = (raw(x-1,y) + raw(x+1,y))/2.0;
        }
      }
		}
	}


  return result;
}

Image edgeBasedGreenDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                             int offsetRedY, int offsetBlueX, int offsetBlueY) {
  // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns an rgb image
  // using edge-based green demosaicing for the green channel and
  // simple interpolation to demosaic the red and blue channels
  Image result = Image(raw.width(), raw.height(), 3);

  Image green = edgeBasedGreen(raw, offsetGreen);
  Image blue = basicRorB(raw, offsetBlueX, offsetBlueY);
  Image red = basicRorB(raw, offsetRedX, offsetRedY);

  // RGB
  for (int y = 0; y < raw.height(); y++) { 
		for (int x = 0; x < raw.width(); x++)  {
      if ((x == 0 or x == raw.width()-1) and (y == 0 or y == raw.height()-1)){
        result(x,y,0) = raw(x,y);
        result(x,y,1) = raw(x,y);
        result(x,y,2) = raw(x,y);
      }
      result(x,y,0) = red(x,y);
      result(x,y,1) = green(x,y);
      result(x,y,2) = blue(x,y);
		}
	}
  return result;

  //question: i notice a lot more green in the picture than before - this might be able to be fixed with finding the
  // outlying condition and finding a new value for it
}

Image greenBasedRorB(const Image &raw, Image &green, int offsetX, int offsetY) {
  // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns a single-channel
  // 2D image corresponding to the red or blue channel using green based
  // interpolation

  Image green_3d = Image(raw.width(), raw.height(), 3);
  for (int y = 0; y < raw.height(); y++) {
    for (int x = 0; x < raw.width(); x++) {
      for (int z = 0; z < raw.channels(); z++){
        green_3d(x,y,z) = green(x,y);
      }
    }
  }
  // basicRB(RAW - Green) + Green
  Image rb_minus_green = basicRorB(raw - green_3d, offsetX, offsetY);
  return rb_minus_green + green;
}

Image improvedDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                       int offsetRedY, int offsetBlueX, int offsetBlueY) {
  // // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns an rgb image
  // using edge-based green demosaicing for the green channel and
  // simple green based demosaicing of the red and blue channels
  Image result = Image(raw.width(), raw.height(), 3);

  Image green = edgeBasedGreen(raw, offsetGreen);
  Image blue = greenBasedRorB(raw, green, offsetBlueX, offsetBlueY);
  Image red = greenBasedRorB(raw, green, offsetRedX, offsetRedY);

  // RGB
  for (int y = 0; y < raw.height(); y++) { 
		for (int x = 0; x < raw.width(); x++)  {
      if ((x == 0 or x == raw.width()-1) and (y == 0 or y == raw.height()-1)){
        result(x,y,0) = raw(x,y);
        result(x,y,1) = raw(x,y);
        result(x,y,2) = raw(x,y);
      }
      result(x,y,0) = red(x,y);
      result(x,y,1) = green(x,y);
      result(x,y,2) = blue(x,y);
		}
	}
  return result;
}
