// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include <TM1637.h>

#define CLK 7
#define DIO 8
TM1637 tm1637(CLK,DIO);

#define DHTPIN 2     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

bool point = true;
int red_light_pin= 9;
int green_light_pin = 10;
int blue_light_pin = 11;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  tm1637.point(true);
  dht.begin();

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  for (int i = 0; i < 30; i++) {
    ledTemp(i);
    Serial.println(i);
    delay(100);
  ledTemp(t);
}

void ledTemp(float t) {
  int min = 5;
  int max = 25;
  if (t < min) {
    t = min;
  }
  if (t > max) {
    t = max;
  }
  int red = 0;
  if (t >= min) {
    red = int((t - min) * (255/(max - min)));
  }
  int green = 0;
  int blue = 0;
  if (t <= max) {
    blue = int((max - t) * (255/(max - min)));
  }

  RGB_color(red, green, blue);
}

void displayTemp(float t) {
  String ts = String(t);
  tm1637.display(0,ts.substring(0,1).toInt());
  tm1637.display(1,ts.substring(1,2).toInt());
  tm1637.display(2,ts.substring(2,3).toInt());
  tm1637.display(3,ts.substring(3,4).toInt());
}

