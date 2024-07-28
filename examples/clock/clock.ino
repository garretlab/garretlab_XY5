/*
 * Udp NTP Client is from WiFiUdpNtpClient
*/

#include <WiFiS3.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;  // your network SSID (name)
char pass[] = SECRET_PASS;  // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

unsigned int localPort = 2390;             // local port to listen for UDP packets
IPAddress timeServer(162, 159, 200, 123);  // pool.ntp.org NTP server
const int NTP_PACKET_SIZE = 48;            // NTP timestamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE];        //buffer to hold incoming and outgoing packets

WiFiUDP Udp;

#include <garretlab_XY5.h>

garretlab_XY5 xy5;
extern const struct Font Font_5x7_XY5_digits;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);

    delay(10000);
  }

  Serial.println("Connected to WiFi");

  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);

  Serial1.begin(57600);
  xy5.begin(&Serial1, 1);
  xy5.stroke(1, 1, 1);
  xy5.background(0, 0, 0);
  xy5.textFont(Font_5x7_XY5_digits);
}

void loop() {
  unsigned long secsSince1900 = getNTPTime();
  Serial.print("Seconds since Jan 1 1900 = ");
  Serial.println(secsSince1900);

  Serial.print("Unix time = ");
  const unsigned long seventyYears = 2208988800UL;
  unsigned long epoch = secsSince1900 - seventyYears + 9 * 3600;
  Serial.println(epoch);

  Serial.print("The UTC time is ");
  Serial.print((epoch % 86400L) / 3600);
  Serial.print(':');
  if (((epoch % 3600) / 60) < 10) {
    Serial.print('0');
  }
  Serial.print((epoch % 3600) / 60);
  Serial.print(':');
  if ((epoch % 60) < 10) {
    Serial.print('0');
  }
  Serial.println(epoch % 60);  // print the second

  Serial.println("xy5");
  Serial.println((epoch % 86400L) / 36000);
  Serial.println(((epoch % 86400L) / 3600) % 10);
  Serial.println(((epoch % 3600L) / 600));
  Serial.println(((epoch % 3600L) / 60) % 10);

  xy5.clear();
  xy5.beginText(0, 0, 1, 1, 1);
  xy5.text(":", 11, 0);
  if ((epoch % 86400L) / 36000) {
    xy5.text(String((epoch % 86400L) / 36000), 1, 0);
  }
  xy5.text(String(((epoch % 86400L) / 3600) % 10), 7, 0);
  xy5.text(String((epoch % 3600L) / 600), 16, 0);
  xy5.text(String(((epoch % 3600L) / 60) % 10), 22, 0);
  xy5.endDraw();

  delay((60 - (epoch % 60)) * 1000);
}

unsigned long getNTPTime() {
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;  // LI, Version, Mode
  packetBuffer[1] = 0;           // Stratum, or type of clock
  packetBuffer[2] = 6;           // Polling Interval
  packetBuffer[3] = 0xEC;        // Peer Clock Precision
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;

  Udp.beginPacket(timeServer, 123);  //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
  delay(1000);
  if (Udp.parsePacket()) {
    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    return secsSince1900;
  } else {
    return 0;
  }
}