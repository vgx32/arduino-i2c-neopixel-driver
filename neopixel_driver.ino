// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
// #include <Wire.h>
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

byte _displayBuf[] ={ 20, 10, 0,
                20, 20, 20,
                0, 20, 20,
                20, 0, 10};
              
int _displayBufCt = NUMPIXELS * 3;
int _curPixValsIndex = 0;
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

void setCurMode(byte m) {
  _curMode = m;
}

String _pixelBuf = "";

void setup() {

  pixels.begin(); // This initializes the NeoPixel library.

  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output

// re-add when raspberry pi is fixed
  // initialize i2c as slave
  // Wire.begin(SLAVE_ADDRESS);
  
  // // define callbacks for i2c communication
  // Wire.onReceive(receiveData);
  // Wire.onRequest(sendData);

  _pixelBuf.reserve(_displayBufCt + 1);
  Serial.println("Ready!");
  
}

bool stringComplete = false;

void loop() {
  if (stringComplete) {
    Serial.println("got a buf!");
    Serial.println(_pixelBuf);
    // clear the string:
    updateDisplayBuffer();
    _pixelBuf = "";
    stringComplete = false;
  }
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
    r = _displayBuf[bufPosition*3];
    g = _displayBuf[bufPosition*3 + 1];
    b = _displayBuf[bufPosition*3 + 2];
    setPixel(pixel, r, g, b);
  
}

void cycleAllAtOnce(int brightness) {

  for (int i =0; i < _displayBufCt/3; i++)
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

void updateDisplayBuffer(){
  setCurMode(_pixelBuf[0]);
  for (int i = 0; i < _displayBufCt; i++) {
    _displayBuf[i] = _pixelBuf[i+1];
  }
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    byte inVal = Serial.read();
    // add it to the inputString:
    _pixelBuf += (char)inVal;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if ((char)inVal == '\n') {
      stringComplete = true;
    }
  }
}
// re-add when working raspberry pi code
// callback for received data
// void receiveData(int byteCount){
//   byte b = 0;
//   while(Wire.available()) {
//     b = Wire.read();
//     Serial.print("data received: ");
//     Serial.println(b);
//     if(isMode(b)){
//       setCurMode(b);
//     }
    
   
//   }
// }

// // callback for sending data
// void sendData(){
// //  Wire.write(number);
// }



