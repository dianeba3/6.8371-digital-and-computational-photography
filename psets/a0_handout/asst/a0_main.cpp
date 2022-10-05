#include "a0.h"

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of a0.cpp and Image.cpp
int main() {
  // --------- HANDOUT  PS00 ------------------------------

  cout << getSum(1.0, 2.0) << endl;
  helloworld(1.0, 2.0);

  SimpleImage im = SimpleImage(5, 3);

  im.SimpleImage::make_rectangle();

  im.print_to_terminal();

  cout << im.SimpleImage::compute_average() << endl;

  

  // Example Test
  // Read an Image
  // Image im("./Input/Cambridge1.png");

  // Write an Image
  // cout << "Image size: " << im.width() << ", "
  //      << im.height() << ", " << im.channels() << endl;
  // im.write("./Output/SameAsInput.png");
}
