#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

// screen resolution set
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// sensor and leds
#define DHTPIN 2     
#define DHTTYPE DHT22   
#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN 3

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // screen initialization and sanity check
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  // text initialization
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("System Starting...");
  display.display();

  // sets led pins as outputs 
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  //sets a delay of 2 seconds between readings from the dht22
  delay(2000);

  // humidity/temperature readings
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // sanity check
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Sensor Error!");
    display.display();
    return;
  }

  // print loop for console 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("c");

  //oled display 
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);

  //oled printing current temps
  display.print("Temp:  ");
  display.print(t);
  display.println(" C");
  
  display.print("Humid: ");
  display.print(h);
  display.println(" %");
  
  display.println(); 

  // loop for LED and OLED screen
  
  // rgb set for high temp
  if (h > 80.0 || t > 32.0) {
    setRGB(255, 0, 0); 
    Serial.println("LED: RED (High Humidity and/or Temperature)");

    // high parameters
    if (h > 80.0 && t > 32.0) {
      display.println("WARN: Both Too High!");
    } else if (t > 32.0) {
      display.println("WARN: Temp Too High!");
    } else if (h > 80.0) {
      display.println("WARN: Humid Too High!");
    }
    
  } 
  // rgb set for normal conditions
  else if ((h >= 60.0 && h <= 80.0) && (t >= 26.0 && t <= 32.0)) {
    setRGB(0, 255, 0);
    Serial.println("LED: GREEN (Normal Humidity and Temperature)");
    display.println("Status: Normal");
    
  } 
  //rgb set for low temp
  else {
    setRGB(0, 0, 255);
    Serial.println("LED: BLUE (Low Humidity and/or Temperature)");

    // low parameters
    if (h < 60.0 && t < 26.0) {
      display.println("WARN: Both Too Low!");
    } else if (t < 26.0) {
      display.println("WARN: Temp Too Low!");
    } else if (h < 60.0) {
      display.println("WARN: Humid Too Low!");
    }
  }

  // Actually push the text to the screen
  display.display(); 
}

// function to set rgb colors
void setRGB(int r, int g, int b) {
  // since its a common anode, we subtract 255 from the rgb colors since 0 is highest
  analogWrite(RED_PIN, 255 - r);
  analogWrite(GREEN_PIN, 255 - g);
  analogWrite(BLUE_PIN, 255 - b);
}