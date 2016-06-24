#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

const char* ssid = "Lobster";
const char* password = "=urFancyL0bst3rPassPhras3";

#define PIN 2

#define BULK_SIZE 5

const int FAILURE[3]    = {255, 0, 0};       // red
const int SUCCESS[3]    = {0, 255, 0};       // green
const int BACKGROUND[3] = {255, 255, 255};   // white
const int UNSTABLE[3]   = {255, 255, 0};     // yellow
const int ABORTED[3]    = {50, 50, 50};      // grey

// progress = 0 => in progress after failure
// progress = 1 => in progress after success
// progress = 2 => in progress after unstable
// progress = 3 => in progress after abort
// progress = 4 => switch to failed
// progress = 5 => switch to succeeded
// progress = 6 => switch to unstable
// progress = 7 => switch to aborted
int progress = -1;
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

WiFiServer server(80);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  //Webserver setup:
  Serial.begin(115200);
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  //LED circle setup
  strip.setBrightness(50);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    switch(progress){
      case 0:
        spinner(FAILURE, BACKGROUND, BULK_SIZE, 50);
        break;
      case 1:
        spinner(SUCCESS, BACKGROUND, BULK_SIZE, 50);
        break;
      case 2:
        spinner(UNSTABLE, BACKGROUND, BULK_SIZE, 50);
        break;
      case 3:
        spinner(ABORTED, BACKGROUND, BULK_SIZE, 50);
        break;
      case 4:
        for(short i=0;i<6;i++){
          blink(FAILURE, BACKGROUND, 500);
        }
        progress=8;
        break;
      case 5:
      for(short i=0;i<6;i++){
          blink(SUCCESS, BACKGROUND, 500);
        }
        progress=9;
        break;
      case 6:
        for(short i=0;i<6;i++){
          blink(UNSTABLE, BACKGROUND, 500);
        }
        progress=10;
        break;
      case 7:
        for(short i=0;i<6;i++){
          blink(ABORTED, BACKGROUND, 500);
        }
        progress=11;
        break;
      case 8:
        setDarkerBackgroundColor(FAILURE);
        break;
      case 9:
        setDarkerBackgroundColor(SUCCESS);
        break;
      case 10:
        setDarkerBackgroundColor(UNSTABLE);
        break;
      case 11:
        setDarkerBackgroundColor(BACKGROUND);
        break;
      default:
        strip.show();
    }
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request
  if (request.indexOf("/progress?status=FAILURE") != -1)  {
    progress = 0;
  }
  if (request.indexOf("/progress?status=SUCCESS") != -1)  {
    progress = 1;
  }
  if (request.indexOf("/progress?status=UNSTABLE") != -1)  {
    progress = 2;
  }
  if (request.indexOf("/progress?status=ABORTED") != -1)  {
    progress = 3;
  }
  if (request.indexOf("/FAILURE") != -1)  {
    progress = 4;
  }
  if (request.indexOf("/SUCCESS") != -1)  {
    progress =5;
  }
  if (request.indexOf("/UNSTABLE") != -1)  {
    progress = 6;
  }
  if (request.indexOf("/ABORTED") != -1)  {
    progress = 7;
  }
  Serial.print("progress status: ");
  Serial.println(progress);
  
  // Some example procedures showing how to display to the pixels:
  //spinner(BACKGROUND, SUCCESS, 3, 50);
  // let all LEDs blink
  //blink(FAILURE, SUCCESS, 500);
}

// Fill the dots one after the other with a color
void spinner(const int Background[], const int Pixel[], int bulkSize, short wait) {
  for(int i=1; i<=strip.numPixels(); i++) {
    setDarkerBackgroundColor(Background);
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
 setDarkerBackgroundColor(color_before);
 delay(wait);
}

void afterBlink(const int blink_color[]){
 setBackgroundColor(blink_color);
}

void setBackgroundColor(const int color[]) {
 for(short i=0; i<strip.numPixels(); i++) {
   strip.setPixelColor(i, strip.Color(color[0],color[1],color[2]));
 }
 strip.show();
}

void setDarkerBackgroundColor(const int color[]) {
 for(short i=0; i<strip.numPixels(); i++) {
   strip.setPixelColor(i, strip.Color(color[0]/3,color[1]/3,color[2]/3));
 }
 strip.show();
}
