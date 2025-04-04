#include <SoftwareSerial.h>
#include <DHT.h>

// DHT sensor configuration
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Define pins for sensors
#define RAIN_SENSOR_ANALOG_PIN A0
#define RAIN_SENSOR_DIGITAL_PIN 13
#define FLAME_SENSOR_PIN 4

// Define LED pins
#define TEMP_LED 5      // LED for temperature alert
#define RAIN_LED 6      // LED for rain detection
#define FLAME_LED 7     // LED for flame detection
#define CONTROL_LED 8   // LED controlled via mobile

// Bluetooth module (HC-05) connected to pins 10 (RX), 11 (TX)
SoftwareSerial BTSerial(10, 11);

void setup() {
  Serial.begin(9600);   // Serial Monitor for debugging
  BTSerial.begin(9600); // HC-05 Bluetooth Communication

  dht.begin(); // Initialize DHT sensor

  pinMode(RAIN_SENSOR_ANALOG_PIN, INPUT);
  pinMode(RAIN_SENSOR_DIGITAL_PIN, INPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);

  // LED setup
  pinMode(TEMP_LED, OUTPUT);
  pinMode(RAIN_LED, OUTPUT);
  pinMode(FLAME_LED, OUTPUT);
  pinMode(CONTROL_LED, OUTPUT);

  // Use INPUT_PULLUP to slightly reduce TX voltage, protecting HC-05 RX pin
  pinMode(10, INPUT_PULLUP);

  Serial.println("Bluetooth Module Initialized. Ready to send data!");
  BTSerial.println("Bluetooth Module Initialized. Ready to send data!");
}

void loop() {
  // Read DHT sensor values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Validate DHT sensor readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT sensor!");
    BTSerial.println("Error reading DHT sensor!");
    return;
  }

  // Read rain sensor values
  int rainAnalogValue = analogRead(RAIN_SENSOR_ANALOG_PIN);
  int rainDigitalValue = digitalRead(RAIN_SENSOR_DIGITAL_PIN);

  // Read flame sensor values (digital)
  int flameSensorValue = digitalRead(FLAME_SENSOR_PIN);

  // LED Logic
  digitalWrite(TEMP_LED, temperature > 30 ? HIGH : LOW);  // Turn ON if temp > 30°C
  digitalWrite(RAIN_LED, rainAnalogValue < 400 ? HIGH : LOW);  // Turn ON if rain detected
  digitalWrite(FLAME_LED, flameSensorValue == LOW ? HIGH : LOW); // Turn ON if flame detected

  // Format sensor data into a structured message
  String sensorData = "Temperature: " + String(temperature) + " °C\n";
  sensorData += "Humidity: " + String(humidity) + " %\n";
  sensorData += "Rain Detected: " + String(rainAnalogValue > 400 ? "No" : "Yes") + "\n";
  sensorData += "Flame Detected: " + String(flameSensorValue == HIGH ? "No" : "Yes") + "\n";
  sensorData += "Rain Analog Value: " + String(rainAnalogValue) + "\n";

  // Print data to Serial Monitor
  Serial.println(sensorData);

  // Send data via Bluetooth
  BTSerial.println(sensorData);

  // Check for data received via Bluetooth (Control LED)
  if (BTSerial.available()) {
    String receivedData = BTSerial.readStringUntil('\n');
    receivedData.trim();
    Serial.print("Received via Bluetooth: ");
    Serial.println(receivedData);

    if (receivedData == "1") {
      digitalWrite(CONTROL_LED, HIGH);
    } else if (receivedData == "0") {
      digitalWrite(CONTROL_LED, LOW);
    }
  }

  // Check for data received from Serial Monitor and send via Bluetooth
  if (Serial.available()) {
    String sendData = Serial.readStringUntil('\n');
    Serial.print("Sending to Bluetooth: ");
    Serial.println(sendData);
    BTSerial.println(sendData);
  }

  delay(2000); // Delay for readability
}
