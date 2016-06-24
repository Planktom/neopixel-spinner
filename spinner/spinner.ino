#include <Adafruit_NeoPixel.h>

#define PIN 6
const int FAILURE[3]    = {255, 0, 0};       // red
const int SUCCESS[3]    = {0, 255, 0};       // green
const int BACKGROUND[3] = {255, 255, 255};   // white
const int UNSTABLE[3]   = {255, 255, 0};     // yellow
const int ABORTED[3]    = {50, 50, 50};      // grey

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.setBrightness(50);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  spinner(BACKGROUND, SUCCESS, 3, 50);
  // let all LEDs blink
  blink(FAILURE, SUCCESS, 500);
}

// Fill the dots one after the other with a color
void spinner(const int Background[], const int Pixel[], int bulkSize, short wait) {
  for(int i=1; i<=strip.numPixels(); i++) {
    setBackgroundColor(Background);
    for(int j=0; j<bulkSize; j++) {
      strip.setPixelColor(((i+j)) % strip.numPixels(), strip.Color(Pixel[0]/(bulkSize-j), Pixel[1]/(bulkSize-j), Pixel[2]/(bulkSize-j)));
    }
    strip.show();
    delay(wait);
  }
}

// let all LEDs blink with the color from the build before
void blink(const int blink_color[],const int color_before[], short wait) {
  setBackgroundColor(blink_color);
  delay(wait);
  setBackgroundColor(color_before);
  delay(wait);
}

void setBackgroundColor(const int color[]) {
  for(short i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(color[0],color[1],color[2]));
  }
  strip.show();
}
