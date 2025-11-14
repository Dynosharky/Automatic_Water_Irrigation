#include <SoftwareSerial.h>

// Pin definitions
const int moistureSensorPin = A0; // Analog pin connected to the soil moisture sensor
const int relayPin = 7;           // Digital pin connected to the relay module
const int SIM800_TX = 2;          // Arduino RX, SIM800 TX
const int SIM800_RX = 3;          // Arduino TX, SIM800 RX

// Threshold value for soil moisture level
int moistureThreshold = 500; // Adjust based on calibration

SoftwareSerial sim800(SIM800_TX, SIM800_RX);

String phoneNumber = "+919360480748"; // Authorized phone number
bool pumpManualOverride = false;
bool pumpStatus = false; // false = OFF, true = ON

void setup() {
  pinMode(moistureSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Start with the relay off
  
  Serial.begin(9600);   // For debugging via serial monitor
  sim800.begin(9600);   // Initialize SIM800 module
  
  // Wait for SIM800 module to initialize
  delay(1000);
  
  // Set SMS mode to text
  sim800.println("AT+CMGF=1");
  delay(1000);
  // Set SMS to show immediately
  sim800.println("AT+CNMI=1,2,0,0,0");
  delay(1000);
}

void loop() {
  int moistureLevel = analogRead(moistureSensorPin); // Read the moisture level
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel); // Print the moisture level for debugging

  if (sim800.available() > 0) {
    String sms = sim800.readString();
    Serial.println("SMS Received: " + sms);

    if (sms.indexOf(phoneNumber) >= 0) {
      if (sms.indexOf("PUMP ON") >= 0) {
        digitalWrite(relayPin, HIGH); // Turn on the pump
        pumpStatus = true;
        pumpManualOverride = true;
        Serial.println("Pump turned ON via SMS");
      } else if (sms.indexOf("PUMP OFF") >= 0) {
        digitalWrite(relayPin, LOW); // Turn off the pump
        pumpStatus = false;
        pumpManualOverride = true;
        Serial.println("Pump turned OFF via SMS");
      } else if (sms.indexOf("STATUS") >= 0) {
        sendSMS(phoneNumber, "Moisture Level: " + String(moistureLevel));
      }
    }
  }

  if (!pumpManualOverride) {
    if (moistureLevel < moistureThreshold && !pumpStatus) {
      digitalWrite(relayPin, HIGH); // Turn on the relay (and the motor)
      pumpStatus = true;
      sendSMS(phoneNumber, "No Water. Pump turned ON");
      Serial.println("Pump ON due to low moisture");
    } else if (moistureLevel >= moistureThreshold && pumpStatus) {
      digitalWrite(relayPin, LOW); // Turn off the relay (and the motor)
      pumpStatus = false;
      sendSMS(phoneNumber, "Pump turned OFF");
      Serial.println("Pump OFF due to sufficient moisture");
    }
  }

  delay(1000); // Wait for a second before taking another reading
}

void sendSMS(String number, String text) {
  sim800.print("AT+CMGS=\"");
  sim800.print(number);
  sim800.println("\"");
  delay(100);
  sim800.println(text);
  delay(100);
  sim800.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}
