#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1

// DHT
#define PIN 4
#define TYPE DHT22

// Initialization
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);             
DHT dht(PIN, TYPE);

void setup() {
  Serial.begin(9600);

  dht.begin();

}

void loop() {



}

//THING TO MAYBE USE
//void display(void);
//void clearDisplay(void);
//void drawPixel(int16_t x, int16_t y, uint16_t color);
//bool getPixel(int16_t x, int16_t y);
//DHT(uint8_t pin, uint8_t type, uint8_t count = 6);
//float readTemperature(bool S = false, bool force = false);
//float readHumidity(bool force = false);
//float convertCtoF(float);
//float convertFtoC(float);