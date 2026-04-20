#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// oled screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// dht sensor initialization
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// rgb pins
#define RGB_RED_PIN   11
#define RGB_BLUE_PIN  12  
#define RGB_GREEN_PIN 13  

// function to set led color
void setLedColor(bool red, bool green, bool blue) {
  digitalWrite(RGB_RED_PIN,   red   ? LOW : HIGH);
  digitalWrite(RGB_GREEN_PIN, green ? LOW : HIGH);
  digitalWrite(RGB_BLUE_PIN,  blue  ? LOW : HIGH);
}

void setup() {
  Serial.begin(9600);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // sets pins to outputs and inititializes screen info
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  setLedColor(false, false, false);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(F("Initializing..."));
  display.display();
  delay(2000);
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // sanity checks
  if (isnan(h) || isnan(t)) {
    setLedColor(true, false, false); // red error led
    display.clearDisplay();
    display.setCursor(0, 10);
    display.setTextSize(1);
    display.println(F("Read Error!")); // prints error on screen in case
    display.display();
    delay(500);
    setLedColor(false, false, false);
    return;
  }

  // high/low conditions
  bool tHigh = t > 32.0;
  bool tLow  = t < 26.0;
  bool hHigh = h > 80.0;
  bool hLow  = h < 60.0;

  // led colors controlled by the function above
  if (tHigh || hHigh) {
    setLedColor(true, false, false); // red
  } else if (tLow || hLow) {
    setLedColor(false, false, true); // blue
  } else {
    setLedColor(false, true, false); // green
  }

  // oled screen printing
  display.clearDisplay();

  // print temp and humidity data
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(F("T:"));
  display.print(t, 1);
  display.print((char)247); 
  display.print(F("C"));
  
  display.setCursor(0, 18);
  display.print(F("H:"));
  display.print(h, 1);
  display.print(F(" %"));

  // print high/low messages
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print(F("STATUS:"));
  display.setCursor(0, 50);

  // oled messages
  if (tHigh && hHigh) {
    display.print(F("Temp & Humid Too High"));
  } 
  else if (tLow && hLow) {
    display.print(F("Temp & Humid Too Low"));
  } 
  else if (tHigh && hLow) {
    display.print(F("Temp High, Humid Low"));
  } 
  else if (tLow && hHigh) {
    display.print(F("Temp Low, Humid High"));
  } 
  else if (tHigh) {
    display.print(F("Temp Too High"));
  } 
  else if (tLow) {
    display.print(F("Temp Too Low"));
  } 
  else if (hHigh) {
    display.print(F("Humidity Too High"));
  } 
  else if (hLow) {
    display.print(F("Humidity Too Low"));
  } 
  else {
    display.print(F("Temp & Humid Normal"));
  }

  
  display.display();
}