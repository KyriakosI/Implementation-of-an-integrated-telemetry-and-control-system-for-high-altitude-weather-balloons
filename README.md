This project uses an Arduino Uno with a BMP180, GPS module NEO-6M and NRF24L01+(Should have a DHT11 to measure humidity aswell but don't have one at the moment). A RPi5. An ESP32 with a NRF24L01+ connected on the RPi5.

Arduino Uno should be able to collect data such as temperature, atmospheric pressure, humidity, gps location(latitude and longitude), altitude and speed. Uno transmits these data via NRF24L01 and ESP32 receives them. RPi5 gets the data via Serial, displays the data, 
creates plots for the data and tracks the balloon.

Uno should be attached on a weather balloon.

More info about the project and circuits of the project will come soon.
