#include "Adafruit_NeoPixel.h"
#include "protocol.h"
#define PIN 8


//this union is used to read shorts from serial
union Short
{
  short value;
  uint8_t data[2];
};

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

typedef void (*void_ptr_t)( void );
void_ptr_t commandHandlers[NUM_COMMANDS];


//waits until at least count bytes are availabe from the serial port
void waitForBytes(const uint8_t count);

void handleSetColor();
void handleFlush();

void setup() 
{
  commandHandlers[SET_COLOR] = handleSetColor;
  commandHandlers[FLUSH] = handleFlush;
  strip.begin();
  for(int i = 0; i < 60; ++i)
  {//initialize with white pixels
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(115200);
  
}

void loop() 
{ 
  //wait for the next command and execute it
  if(Serial.available() > 0)
  {
    uint8_t command = Serial.read();
    if(command < NUM_COMMANDS)
    {
      commandHandlers[command]();
    }
  }
}

void handleSetColor()
{
  waitForBytes(5);//5 byte = 1 short and 3 byte RGB
  Short pixelIndex;
  Serial.readBytes((char*)&pixelIndex, 2);
  const uint8_t r = Serial.read();
  const uint8_t g = Serial.read();
  const uint8_t b = Serial.read();
  strip.setPixelColor(pixelIndex.value, strip.Color(r, g, b));
}

void handleFlush()
{
  strip.show();  
}


void waitForBytes(const uint8_t count)
{
  while(Serial.available() < count);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

