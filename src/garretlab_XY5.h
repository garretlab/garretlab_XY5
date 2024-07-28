#ifndef GARRETLAB_XY5_H
#define GARRETLAB_XY5_H

#include <garretlab_ArduinoGraphics.h>

class garretlab_XY5 : public garretlab_ArduinoGraphics {
 public:
  garretlab_XY5();                                          // Constructor.
  int begin(Stream *port, byte address);                    // Begin matrix.
  void endDraw();                                           // Display.
  void clear();                                             // Clear display.
  void set(int x, int y, uint8_t r, uint8_t g, uint8_t b);  // Set pixels.
  void endText(int scrollDirection = NO_SCROLL);            // Display text.

 private:
  static const uint8_t canvasWidth = 28;
  static const uint8_t canvasHeight = 7;
  uint8_t address;
  uint8_t frameHolder[canvasWidth];
  Stream *port;

  void renderBitmap();  // Display.
};

#endif /* GARRETLAB_XY5_H */