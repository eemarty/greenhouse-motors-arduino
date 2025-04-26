// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>
DHT11 dht11(12);

//pin definitions on the arduino Nano board. "use old bootloader"
int relay_1 = 1;
int relay_2 = 2;
int vent1 = 3;
int vent2 = 4;
int side2_DN = 5;
int side2_UP = 6;
int side1_DN = 7;
int side1_UP = 8;

//this variable keeps track of how far the sides have opened so the relays dont keep trying to open the sides more.
//If the sides are open and it's cold when arduino is powered on there could be an issue where they wouldn't automatically roll down.
int side1_percentopen = 0;
int side2_percentopen = 0;

void setup() {
  Serial.begin(9600);

  //relays are active low. HIGH is off and LOW is on.
  pinMode(relay_1, OUTPUT);  digitalWrite(relay_1, HIGH);
  pinMode(relay_2, OUTPUT);  digitalWrite(relay_2, HIGH);
  pinMode(vent1, OUTPUT);  digitalWrite(vent1, HIGH);
  pinMode(vent2, OUTPUT);  digitalWrite(vent2, HIGH);
  pinMode(side1_UP, OUTPUT);  digitalWrite(side1_UP, HIGH);
  pinMode(side1_DN, OUTPUT);  digitalWrite(side1_DN, HIGH);
  pinMode(side2_UP, OUTPUT);  digitalWrite(side2_UP, HIGH);
  pinMode(side2_DN, OUTPUT);  digitalWrite(side2_DN, HIGH);
}

void loop() {
    int temperatureC, humidity;
    float temperatureF;

    // Attempt to read the temperature and humidity values from the DHT11 sensor.
    int result = dht11.readTemperatureHumidity(temperatureC, humidity);
    
    // If the reading is successful, print the temperature and humidity values in CSV format.
    if (result == 0) {
      temperatureF = 1.8*temperatureC+32;
      Serial.print("tempF, RH: ");
      Serial.print(temperatureF);
      Serial.print(", ");
      Serial.println(humidity);
    } else {
      // Print error message based on the error code.
      Serial.println(DHT11::getErrorString(result));
    }

    //print some data to the serial output each cycle
    //Serial.print("side1_percentopen: ");
    //Serial.println(side1_percentopen);   
    //Serial.print("side2_percentopen: ");
    //Serial.println(side2_percentopen);

    if (temperatureF > 80) { 
      //roll up each side 1 step unless its already 100% open
      if (side1_percentopen < 100) {
        digitalWrite(side1_UP, LOW);
        side1_percentopen += 10;    
      }
      if (side2_percentopen < 100) {
        digitalWrite(side2_UP, LOW);
        side2_percentopen += 10;
      }      
      //Motors stay on for 12 seconds then turn off
      delay(12000);
      digitalWrite(side1_UP, HIGH);
      digitalWrite(side2_UP, HIGH);      
    }
    
    if (temperatureF < 77) {
      //roll up each side 1 step unless its already 100% closed
      if (side1_percentopen > 0) {
        digitalWrite(side1_DN, LOW);
        side1_percentopen -= 10;
      }
      if (side2_percentopen > 0) {
        digitalWrite(side2_DN, LOW);
        side2_percentopen -= 10;
      }      
      //Motors stay on for 12 seconds then turn off
      delay(12000);
      digitalWrite(side1_DN, HIGH);
      digitalWrite(side2_DN, HIGH);
    }

  // delay 100s so it only adjusts about one step every 2 minutes.
  delay(100000);
}