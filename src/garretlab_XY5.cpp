#include <garretlab_XY5.h>

const uint8_t XY5_HEADER = 0x80;
const uint8_t XY5_WRITE_COMMAND = 0x83;
const uint8_t XY5_ENDBYTE = 0x8f;

garretlab_XY5::garretlab_XY5()
  : ArduinoGraphics(canvasWidth, canvasHeight) {
}

int garretlab_XY5::begin(Stream *port, uint8_t address) {
  this->port = port;
  this->address = address;

  ArduinoGraphics::begin();
  return 1;
}

void garretlab_XY5::set(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
  if (y >= canvasHeight || x >= canvasWidth || y < 0 || x < 0) {
    return;
  }
  canvasBuffer[y][x] = (r | g | b) > 0 ? 1 : 0;
}

void garretlab_XY5::clear() {
  memset(canvasBuffer, 0, sizeof(canvasBuffer));
}

void garretlab_XY5::endText(int scrollDirection) {
  ArduinoGraphics::endText(scrollDirection);
  renderBitmap();
}

void garretlab_XY5::endDraw() {
  ArduinoGraphics::endDraw();
  renderBitmap();
}

void garretlab_XY5::renderBitmap() {
  canvasToFrame();

  port->write(XY5_HEADER);
  port->write(XY5_WRITE_COMMAND);
  port->write(address);

  for (int x = 0; x < canvasWidth; x++) {
    port->write(frameHolder[x]);
  }

  port->write(XY5_ENDBYTE);
}

void garretlab_XY5::canvasToFrame() {
  for (int x = 0; x < canvasWidth; x++) {
    for (int y = 0; y < canvasHeight; y++) {
      if (canvasBuffer[y][x] == 0) {
        frameHolder[x] &= ~(1 << y);
      } else {
        frameHolder[x] |= (1 << y);
      }
    }
  }
}