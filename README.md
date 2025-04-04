This Arduino-based smart home automation project integrates multiple sensors and a Bluetooth module (HC-05) to enhance safety and convenience. It features:

DHT11 Sensor: Monitors temperature and humidity. If temperature exceeds 30°C, an alert LED turns on.

Rain Sensor: Detects rainfall using both analog and digital signals. If rain is detected, a dedicated LED alerts the user.

Flame Sensor: Detects the presence of fire; triggers an alert LED if flame is detected.

Bluetooth Communication: Sends live sensor data to a smartphone via the HC-05 module. Users can also control an additional LED (CONTROL_LED) by sending commands ("1" to turn ON, "0" to turn OFF).

Serial Monitor Support: For debugging and monitoring through a computer.

This system enables real-time environmental monitoring and remote device control, making it ideal for smart home and safety automation applications.
