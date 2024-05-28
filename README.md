This project uses an Arduino Uno with a BMP180, GPS module NEO-6M and NRF24L01+. A RPi5. An ESP32 with a NRF24L01+ connected on the RPi5.

Arduino Uno should be able to collect data such as temperature, atmospheric pressure, humidity, gps location(latitude and longitude), altitude and speed. Uno transmits these data via NRF24L01 and ESP32 receives them. RPi5 gets the data via Serial, displays the data, 
creates plots for the data and tracks the balloon.


