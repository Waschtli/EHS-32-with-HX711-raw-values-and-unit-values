/*
Testsketch to send HX711 ADC-results as a structured numbers on request by sending "read" to the serial port
    AUTHOR: Rob Tillaart
   PURPOSE: HX711 demo extraction of both channels: A with gain 128 and 64 and channel B with gain 32
   Sources: https://github.com/RobTillaart/HX711
            https://docs.arduino.cc/language-reference/en/functions/communication/serial/readString/
    Boards: Arduino UNO, ESP32S3 Super-Mini as "ESP32S3 Dev Module",
            TTGO T1 T-Display ST7789 OLED based on Adafruit example,
            for ESP32-Boards set "USB CDC On Boot: Enabled" in tools-tab of the Arduino IDE, 
*/

#include <HX711.h>    // https://github.com/RobTillaart/HX710AB
// byte LED_Pin = 48;   // for ESP32-S3 Super Mini = 48   https://www.espboards.dev/esp32/esp32-s3-super-mini/#pinout
float value = 0;
//  pins for HX710 connection
byte dataPin = 13;
byte clockPin = 12;
byte LED_Pin = 48;
HX711 scale;

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(LED_Pin, HIGH);
  // randomSeed(analogRead(0));
  scale.begin(dataPin, clockPin, true);     // true for fast processors like ESP32
  scale.reset();
  delay(500);
  digitalWrite(LED_Pin, LOW);
}

void loop() {
  // Serial.println("Enter data:");
  // while (Serial.available() == 0) {}     //wait for data available
  if (Serial.available() == 0) {
    String teststr = Serial.readString();   //read until timeout
    teststr.trim();                         // remove any \r \n whitespace at the end of the String
    if (teststr == "read")
    {
      digitalWrite(LED_Pin, HIGH);          // start measurements
      scale.set_gain(128);
      while (!scale.is_ready()) {}          // Wait until scale ist readay
      value = (int)scale.read_median(7);
      Serial.print(value);
      // Serial.print(random(-pow(2, 23), pow(2, 23)));
      Serial.print(" ");
      scale.set_gain(32);
      while (!scale.is_ready()) {}          // Wait until scale ist readay
      value = (int)scale.read_median(7);
      Serial.print(value);
      // Serial.print(random(-pow(2, 23), pow(2, 23)));
      Serial.print(" ");
      value = random(0, 10000);
      Serial.print(value/10000, 4);
      Serial.print(" ");
      value = random(0, 10000);
      Serial.println(value/10000, 4);
      delay(100);
      digitalWrite(LED_Pin, LOW);          // measurements finished
    }
    else {
    //  Serial.println("Something else");
    }
  }
}
