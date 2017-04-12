

// testshapes demo for Adafruit RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x32 RGB LED matrix:
// http://www.adafruit.com/products/607

// Written by Limor Fried/Ladyada & Phil Burgess/PaintYourDragon
// for Adafruit Industries.
// BSD license, all text above must be included in any redistribution.

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

// If your 32x32 matrix has the SINGLE HEADER input,
// use this pinout:
#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define OE  9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3
// If your matrix has the DOUBLE HEADER input, use:
//#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
//#define LAT 9
//#define OE  10
//#define A   A3
//#define B   A2
//#define C   A1
//#define D   A0
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false);
int buttonState = 0;
int DisplayState = 2 ;
double setAverage = 0;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  matrix.fillScreen(matrix.Color333(0, 0, 0));
}

void loop() {
  int LEDmatrix[32];
  int matrixHeight = 32;

  while (DisplayState == 1) {
    unsigned long startMillis = millis(); // Start of sample window
    unsigned int peakToPeak = 0;   // peak-to-peak level
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
      sample = analogRead(5);
      if (sample < 1024)  // toss out spurious readings
      {
        if (sample > signalMax)
        {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    double volts = (peakToPeak * 5.0) / 1024;

    int mi = 0;
    int i = 0;
    int reading = analogRead(4) / 2;

    for (mi = 1; mi < matrixHeight; mi = mi + 1) {
      int tempValue;
      if (matrixHeight - mi - 1 >= 0)
        LEDmatrix[matrixHeight - mi] = LEDmatrix[matrixHeight - mi - 1];
    }
    double doubletoadd;
    int newValue;
    doubletoadd = (volts - setAverage) * reading;
    Serial.print("Volts:");
    Serial.println(volts);
    Serial.print("Set Average:");
    Serial.println(setAverage);
    newValue = (int)doubletoadd;
    if (newValue >= 32) {
      newValue = 32;
    }
    else if (newValue <= 1) {
      newValue = 1;
    }
    LEDmatrix[0] = newValue;
    /*
        for(i=0; i < matrixHeight; i=i+1){
          //int newValue = random(31);
          //LEDmatrix1[i] = newValue;
          matrix.drawLine(i, 0, i, 32, matrix.Color333(0, 0, 0));
          matrix.drawLine(i, 0, i, LEDmatrix[i]-3, matrix.Color333(2, 1, 0));
          matrix.drawLine(i, LEDmatrix[i]-3, i, LEDmatrix[i], matrix.Color333(1, 1, 1));
          }*/

    for (i = 0; i < matrixHeight; i = i + 1) {
      int upperheight = 17 + LEDmatrix[i] / 2;
      int lowerheight = 16 - LEDmatrix[i] / 2;
      matrix.drawLine(i, 0, i, 32, matrix.Color333(0, 0, 0));
      //Serial.println(LEDmatrix[i]);

      if (upperheight > 27)
      {
        matrix.drawLine(i, upperheight, i, upperheight - 4, matrix.Color333(0, 0, 2));
        matrix.drawLine(i, lowerheight + 4, i, lowerheight, matrix.Color333(2, 0, 0));
        matrix.drawLine(i, upperheight - 5, i, upperheight - 8, matrix.Color333(0, 0, 3));
        matrix.drawLine(i, lowerheight + 8, i, lowerheight + 5, matrix.Color333(3, 0, 0 ));
        matrix.drawLine(i, upperheight - 9, i, 17, matrix.Color333(2, 2, 2));
        matrix.drawLine(i, 16, i, lowerheight + 9, matrix.Color333(2, 2, 2));
      }
      else if (upperheight > 22)
      {
        matrix.drawLine(i, upperheight, i, upperheight - 3, matrix.Color333(0, 0, 3));
        matrix.drawLine(i, lowerheight + 3, i, lowerheight, matrix.Color333(3, 0, 0));
        matrix.drawLine(i, upperheight - 4, i, 17, matrix.Color333(2, 2, 2));
        matrix.drawLine(i, 16, i, lowerheight + 4, matrix.Color333(2, 2, 2));
      }
      else if (upperheight > 19)
      {
        matrix.drawLine(i, upperheight, i, upperheight - 2, matrix.Color333(0, 0, 3));
        matrix.drawLine(i, lowerheight + 2, i, lowerheight, matrix.Color333(3, 0, 0));
        matrix.drawLine(i, upperheight - 3, i, 17, matrix.Color333(2, 2, 2));
        matrix.drawLine(i, 16, i, lowerheight + 3, matrix.Color333(2, 2, 2));
      }
      else if (upperheight == 17)
      {
        matrix.drawLine(i, 16, i, 16, matrix.Color333(2, 2, 2));
      }
      else {
        matrix.drawLine(i, lowerheight, i, upperheight, matrix.Color333(2, 2, 2));
      }
      //matrix.drawLine(i, 0, i, 32, matrix.Color333(0, 0, 0));
      //matrix.drawLine(i, lowerheight, i, upperheight, matrix.Color333(0, 0, 1));
      //matrix.drawLine(i, LEDmatrix[i]-3, i, LEDmatrix[i], matrix.Color333(1, 0, 1));
    }
    //buttonState = digitalRead(12);
    if (digitalRead(13) == HIGH) {
      delay(250);
      while (millis() - startMillis < 3000) {
        sample = analogRead(5);
        if (sample < 1024)  // toss out spurious readings
        {
          if (sample > signalMax)
          {
            signalMax = sample;  // save just the max levels
          }
          else if (sample < signalMin)
          {
            signalMin = sample;  // save just the min levels
          }
        }
        if (digitalRead(13) != HIGH) {
          DisplayState = 2;
        }
      }
      peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
      volts = (peakToPeak * 5.0) / 1024;
      setAverage = volts;
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      while (digitalRead(13) == HIGH) {
        delay(100);
        if (digitalRead(13) != HIGH) {
        }
      }
    }
  }

  while (DisplayState == 2) {
    unsigned long startMillis = millis(); // Start of sample window
    unsigned int peakToPeak = 0;   // peak-to-peak level
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
      sample = analogRead(5);
      if (sample < 1024)  // toss out spurious readings
      {
        if (sample > signalMax)
        {
          signalMax = sample;  // save just the max levels
        }
        else if (sample < signalMin)
        {
          signalMin = sample;  // save just the min levels
        }
      }
    }
    peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
    double volts = (peakToPeak * 5.0) / 1024;

    int mi = 0;
    int i = 0;
    int reading = analogRead(4) / 2;

    for (mi = 1; mi < matrixHeight; mi = mi + 1) {
      int tempValue;
      if (matrixHeight - mi - 1 >= 0)
        LEDmatrix[matrixHeight - mi] = LEDmatrix[matrixHeight - mi - 1];
    }

    int newValue;
    volts = (volts - setAverage) * reading;
    newValue = (int)volts;
    if (newValue >= 32) {
      newValue = 32;
    }
    LEDmatrix[0] = newValue;

    for (i = 0; i < matrixHeight; i = i + 1) {
      //int newValue = random(31);
      //LEDmatrix1[i] = newValue;
      matrix.drawLine(i, 0, i, 32, matrix.Color333(0, 0, 0));
      matrix.drawLine(i, 0, i, LEDmatrix[i] - 3, matrix.Color333(2, 0, 0));
      if(LEDmatrix[i] > 26){
        matrix.drawLine(i, 0, i, LEDmatrix[i] - 22, matrix.Color333(2, 0, 0));
        matrix.drawLine(i, LEDmatrix[i] - 6, i, LEDmatrix[i] - 21, matrix.Color333(2, 2, 2));
        matrix.drawLine(i, LEDmatrix[i] - 5, i, LEDmatrix[i], matrix.Color333(0, 0, 3));
      }
      else if(LEDmatrix[i] > 14){
        matrix.drawLine(i, 0, i, LEDmatrix[i] - 14, matrix.Color333(2, 0, 0));
        matrix.drawLine(i, LEDmatrix[i] - 4, i, LEDmatrix[i] - 13, matrix.Color333(2, 2, 2));
        matrix.drawLine(i, LEDmatrix[i] - 3, i, LEDmatrix[i], matrix.Color333(0, 0, 3));
      }
      else{
        matrix.drawLine(i, 0, i, LEDmatrix[i] - 3, matrix.Color333(2, 0, 0));
        matrix.drawLine(i, LEDmatrix[i] - 3, i, LEDmatrix[i], matrix.Color333(0, 0, 3));
      }
   }

    buttonState = digitalRead(13);
    if (digitalRead(13) == HIGH) {
      delay(250);
      while (millis() - startMillis < 3000) {
        sample = analogRead(5);
        if (sample < 1024)  // toss out spurious readings
        {
          if (sample > signalMax)
          {
            signalMax = sample;  // save just the max levels
          }
          else if (sample < signalMin)
          {
            signalMin = sample;  // save just the min levels
          }
        }
        if (digitalRead(13) != HIGH) {
          DisplayState = 1;
        }
      }
      peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
      volts = (peakToPeak * 5.0) / 1024;
      setAverage = volts;
      matrix.fillScreen(matrix.Color333(0, 0, 0));
      while (digitalRead(13) == HIGH) {
        delay(100);
      }
    }
  }
}
