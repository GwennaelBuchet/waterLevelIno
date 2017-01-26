
//library : SSD1306 Adafruit
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// pin of the e-tape
#define SENSORPIN A0
#define LEDPIN  12

//value from the e-tape when tank is empty
int _max = 840;
//value from the e-tape when tank is full
int _min = 784;

int _delta = _max - _min; //56
int _alert = 20; //percent
float _percentLeft = 100;

void setup(void) {
  Serial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  pinMode(LEDPIN, OUTPUT);
}

void loop(void) {
  float reading = analogRead(SENSORPIN);

  computePercentLeft(reading);

  displayOnScreen(_percentLeft);

  digitalWrite(LEDPIN, LOW);
  if (_percentLeft <= _alert) {
    digitalWrite(LEDPIN, HIGH);
  }

  delay(1000);
}

void computePercentLeft(float currentValue) {
  float d = currentValue - _min;
  float p = 1 - (d / _delta);
  _percentLeft = p * 100;
}

void displayOnScreen(float p) {
  display.clearDisplay();

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(p);
  display.println(" %");
  display.display();
}

