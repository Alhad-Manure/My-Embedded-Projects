#include <Adafruit_NeoPixel.h>

long randNumber;
#define onBoardLed 1
#define ledPin 4
#define numPixels 5

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(numPixels, ledPin, NEO_GRB + NEO_KHZ800);

const int delayval = 100;
bool ledState = false;
void setup() 
{
  // put your setup code here, to run once:
  pixels.begin();
  pinMode(onBoardLed, OUTPUT);

}

void loop() 
{
  // put your main code here, to run repeatedly:

  int r = random(128);
  int g = random(128);
  int b = random(128);

  for(int i=0; i<numPixels; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();   
  }

  ledState == true?digitalWrite(1, HIGH):digitalWrite(1, LOW);
  ledState = !ledState;   
  delay(delayval);
}
