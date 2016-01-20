// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <Wire.h>
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

int delayval = 500; // delay for half a second

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

byte pixVals[] ={ 20, 0, 0,
                0, 20, 0,
                0, 0, 20,
                20, 0, 10};
              
int pixValsCt = NUMPIXELS * 3;

byte _curMode = 'c';

bool isMode(byte m) {
  return 
    m == 'b' || // alternate between all lit up at once and cycled
    m == 'a' || // light all pixels up at once
    m == 'c'; // (cycle) light each pixel individually
}

byte getCurMode(){
  return _curMode;
}

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  Serial.println("Ready!");
  
}


void loop() {
  int brightness = 1;
  switch(getCurMode()) {
    case 'a':
      cycleAllAtOnce(brightness);
      clearPixels();  
      break;
    case 'b':
      cycleAllAtOnce(brightness);
      clearPixels();  
      cycleOneByOne(brightness);
      clearPixels();
      break;
    case 'c':
      cycleOneByOne(brightness);
      clearPixels();
      break;
    default:
      break;
  }
}

void setPixel(int pixel, int r, int g, int b) {
    pixels.setPixelColor(pixel, pixels.Color(r, g, b));
}

void setPixelFromBuf(int pixel, int bufPosition) {
    byte r,g,b;
    r = pixVals[bufPosition*3];
    g = pixVals[bufPosition*3 + 1];
    b = pixVals[bufPosition*3 + 2];
    setPixel(pixel, r, g, b);
  
}

void cycleAllAtOnce(int brightness) {

  for (int i =0; i < pixValsCt/3; i++)
  {
    setPixelFromBuf(i, i);
  }
  pixels.show();
  delay(delayval * 2);
  
}

void clearPixels() {

  for (int i = 0; i < NUMPIXELS; i++) {
    setPixel(i, 0, 0, 0);
  }
  pixels.show();
}

void cycleOneByOne(int brightness) {
  
  for(int i = 0; i < NUMPIXELS; i++) {
    setPixelFromBuf(i,i);
    pixels.show();
    delay(delayval);
    
    clearPixels();
  }
}

// callback for received data
void receiveData(int byteCount){

  while(Wire.available()) {
    number = Wire.read();
    Serial.print("data received: ");
    Serial.println(number);
    
    if (number == 1){
      if (state == 0){
        digitalWrite(13, HIGH); // set the LED on
        state = 1;
      }
      else
      {
        digitalWrite(13, LOW); // set the LED off
        state = 0;
      }
    }
  }
}

// callback for sending data
void sendData(){
  Wire.write(number);
}



