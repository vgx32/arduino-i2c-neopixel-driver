// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      4

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 750; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}


void loop() {
  int brightness = 1;
  cycleAllAtOnce(brightness);
  clearPixels();  
  cycleOneByOne(brightness);
  clearPixels();
  
}

void setPixel(int pixel, int r, int g, int b) {
    pixels.setPixelColor(pixel, pixels.Color(r, g, b));
}

void cycleAllAtOnce(int brightness) {

  for(int i =  0; i < NUMPIXELS; i++ ){
    setPixel(i, brightness, 0, 0);
  }
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delayval);
  for(int i =  0; i < NUMPIXELS; i++ ){
    setPixel(i, 0, brightness, 0);
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  
  delay(delayval);
  for(int i =  0; i < NUMPIXELS; i++ ){
    setPixel(i, 0, 0, brightness);
  }
  pixels.show(); // This sends the updated pixel color to the hardware.

  delay(delayval);
  
}

void clearPixels() {

  for (int i = 0; i < NUMPIXELS; i++) {
    setPixel(i, 0, 0, 0);
  }
  pixels.show();
}

void cycleOneByOne(int brightness) {
  
  for(int i = 0; i < NUMPIXELS; i++) {
    setPixel(i, brightness, 0, 0);
    pixels.show();
    delay(delayval);
    
    setPixel(i, 0, brightness, 0);
    pixels.show();
    delay(delayval);

    setPixel(i, 0, 0, brightness);
    pixels.show();
    delay(delayval);
    clearPixels();
  }
}

