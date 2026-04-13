#include "DHT.h"


#define DHTPIN 2     
#define DHTTYPE DHT22   
#define RED_PIN 6
#define GREEN_PIN 5
#define BLUE_PIN 3

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // sets led pins as outputs 
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  //sets a delay of 2 seconds between readings from the dht22
  delay(2000);

  // Read humidity
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if reading failed
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println("%");

  // if h(humidity) is higher than 80 set the rgb to red
  if (h > 80.0) {
    
    setRGB(255, 0, 0); 
    Serial.println("LED: RED (High Humidity)");
  } //if h is between 60 and 80 set the rgb to green
  else if (h >= 60.0 && h <= 80.0) {
    
    setRGB(0, 255, 0);
    Serial.println("LED: GREEN (Normal)");
  } //if h is below 60 set it to blue
  else {
    
    setRGB(0, 0, 255);
    Serial.println("LED: BLUE (Low Humidity)");
  }
}

// function to set rgb colors
void setRGB(int r, int g, int b) {
  // since its a common anode, we subtract 255 from the rgb colors since 0 is highest
  analogWrite(RED_PIN, 255 - r);
  analogWrite(GREEN_PIN, 255 - g);
  analogWrite(BLUE_PIN, 255 - b);
}
