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

void setup() 
{
  commandHandlers[SET_COLOR] = handleSetColor;
  Serial.begin(57600);
  strip.begin();
  
  //flash once to indicate reset
  for(int i = 0; i < 60; ++i)
  {//initialize with white pixels
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
  delay(500);
  for(int i = 0; i < 60; ++i)
  {//initialize with white pixels
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();  
  Serial.write(RDY);
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
      Serial.write(RDY);
    }
  }
}

void handleSetColor()
{
  const int num = strip.numPixels();
  for(int i = 0; i < num; ++i)
  {
    waitForBytes(3);//wait for pixel data to arrive
    const uint8_t r = Serial.read();
    const uint8_t g = Serial.read();
    const uint8_t b = Serial.read();
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}


void waitForBytes(const uint8_t count)
{
  while(Serial.available() < count);
}


