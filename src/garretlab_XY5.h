#ifndef GARRETLAB_XY5_H
#define GARRETLAB_XY5_H

#include <ArduinoGraphics.h>

class garretlab_XY5 : public ArduinoGraphics {
public:
  garretlab_XY5();
  int begin(Stream *port, byte address);
  void set(int x, int y, uint8_t r, uint8_t g, uint8_t b);
  void clear();
  void renderBitmap();
  void endText(int scrollDirection = NO_SCROLL);
  void endDraw();

private:
  static const uint8_t canvasWidth = 28;
  static const uint8_t canvasHeight = 7;
  uint8_t address;
  uint8_t canvasBuffer[canvasHeight][canvasWidth] = { { 0 } };
  uint8_t frameHolder[canvasWidth];
  Stream *port;

  void canvasToFrame();
};

#endif /* GARRETLAB_XY5_H */