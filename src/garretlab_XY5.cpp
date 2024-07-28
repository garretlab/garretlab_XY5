#include <garretlab_XY5.h>

const uint8_t XY5_HEADER = 0x80;
const uint8_t XY5_WRITE_COMMAND = 0x83;
const uint8_t XY5_ENDBYTE = 0x8f;

// Constructor.
garretlab_XY5::garretlab_XY5()
    : garretlab_ArduinoGraphics(canvasWidth, canvasHeight) {
}

// Begin matrix.
int garretlab_XY5::begin(Stream *port, uint8_t address) {
  this->port = port;
  this->address = address;

  garretlab_ArduinoGraphics::begin();
  return 1;
}

// Display.
void garretlab_XY5::endDraw() {
  garretlab_ArduinoGraphics::endDraw();
  renderBitmap();
}

// Clear display.
void garretlab_XY5::clear() {
  garretlab_ArduinoGraphics::clear();
  for (int x = 0; x < canvasWidth; x++) {
    frameHolder[x] = 0;
  }
}

// Set pixels.
void garretlab_XY5::set(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  if (y >= canvasHeight || x >= canvasWidth || y < 0 || x < 0) {
    return;
  }

  if (r | g | b) {
    frameHolder[x] |= (1 << y);
  } else {
    frameHolder[x] &= ~(1 << y);
  }
}

// Display text.
void garretlab_XY5::endText(int scrollDirection) {
  garretlab_ArduinoGraphics::endText(scrollDirection);
  renderBitmap();
}

// Display.
void garretlab_XY5::renderBitmap() {
  port->write(XY5_HEADER);
  port->write(XY5_WRITE_COMMAND);
  port->write(address);

  for (int x = 0; x < canvasWidth; x++) {
    port->write(frameHolder[x]);
  }

  port->write(XY5_ENDBYTE);
}