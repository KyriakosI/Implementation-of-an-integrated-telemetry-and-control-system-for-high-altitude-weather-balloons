// Δήλωση βιβλιοθηκών
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Πρέπει να δηλωθεί εδώ για να αρχικοποιηθεί μία φορά και όχι συνέχεια μέσα στο πρόγραμμα
unsigned long lastTransmissionTime = 0; 

const byte address[6] = "00001";

// Αρχικοποίηση DHT11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT HT(DHTPIN,DHTTYPE);

// Αρχικοποίηση BMP180
Adafruit_BMP085 bmp;

// Αρχικοποίηση NRF24L01
#define CE_PIN 8
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);

// Αρχικοποίηση GPS
#define GPSBAUD 9600
#define GPS_RX_PIN 3
TinyGPSPlus gps;
SoftwareSerial gpsSerial(GPS_RX_PIN, -1);  // RX pin, TX pin not used

// Δήλωση μεταβητών για αποθήκευση μετρήσεων
String temperature;
String pressure;
String humidity;
String date;
String gpsLat;
String gpsLong;
String altitude;
String speed;
String time;

void setup() {

  String errorPacket;
  Serial.begin(9600);
  gpsSerial.begin(GPSBAUD);
  bmp.begin();
  HT.begin();
  radio.begin();
  radio.setChannel(6);
  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address);
  radio.stopListening();
  delay(1000);


  // Ελέγχει εαν τα συγκεκριμένα modules είναι καλωδιομένα
  // Eαν όχι στέλνει το μήνυμα μέσω NRF24L01 και τυπώνει ένα μήνυμα ενημερώνοντας τον χρήστη μέσω serial
  while (gpsSerial.available() == 0 || !bmp.begin()) {


    errorPacket = "Hardware issue";
    Serial.println(errorPacket);
    sendData('E', errorPacket);
    delay(2000);
  }

  Serial.println("Sensors are connected. Data collection and transmission will soon begin.");
  delay(5000);
}

void loop() {

  // Ελέγχει αν το NRF24L01 είναι καλωδιομένο, αν όχι τυπώνει μήνυμα στο serial
  if (radio.isChipConnected()) { 

    // Ελέγχει αν το GPS έχει διαθέσιμα data
    if (gpsSerial.available() > 0) { 

      // Διαβάζει ένα byte δεδομένων από το Serial port που συνδέεται με το GPS module
      char gpsInfo = gpsSerial.read(); 

      // Προσπαθεί να αναλύσει το byte δεδομένων ως πληροφορίες GPS 
      // Εάν η ανάλυση είναι επιτυχής, το GPS module έχει αποκωδικοποιήσει και αναλύσει με επιτυχία τα δεδομένα
      // Επίσης ελέγχει εαν πέρασαν 4 δευτερόλεπτα από την προηγούμενη φόρα που έστειλε δεδομένα
      // Εαν όχι τρέχει το πρόγραμμα μέχρι να περάσουν τα 4 δευτερόλεπτα
        if (gps.encode(gpsInfo) && millis() - lastTransmissionTime >= 4000) { // 4000 είναι σε milliseconds

        lastTransmissionTime = millis();          

        prepareDate();
        sendData('D', date);

        prepareTime();
        sendData('T', time);

        prepareHum();
        sendData('H', humidity);

        prepareTemp();
        sendData('C', temperature);

        preparePressure();
        sendData('P', pressure);

        prepareGpsLat();
        sendData('L', gpsLat);

        prepareGpsLong();
        sendData('G', gpsLong);

        prepareAltitude();
        sendData('A', altitude);

        prepareSpeed();
        sendData('S', speed);
      }
    }
  } else {
    Serial.println("NRF24 module is not connected. Board restarting.");
  }
}


void preparePressure() {
  float measure = (bmp.readPressure() / 100);
  pressure = String(measure);
}

void prepareTemp() {
  float measure = bmp.readTemperature();
  temperature = String(measure);
}

void prepareHum() {
  float measure = HT.readHumidity();
  if(!isnan(measure))
  humidity = String(measure);  
}

void prepareDate() {

  String date2 = "";

  if (gps.date.isValid()) {
    int day = gps.date.day();
    int month = gps.date.month();
    int year = gps.date.year();


    if (day < 10) {
      date2 += "0" + String(day) + "/";
    } else date2 += String(day) + "/";


    if (month < 10) {
      date2 += "0" + String(month) + "/";
    } else date2 += String(month) + "/";


    date2 += String(year);

  } else {
    date2 = "01/01/2000";
  }
  date = date2;
}


void prepareTime() {

  String time2 = "";
  if (gps.time.isValid()) {

    int hour = gps.time.hour();
    int minute = gps.time.minute();
    int second = gps.time.second();

    if (hour < 10) {
      time2 += "0" + String(hour) + ":";
    }

    else {
      time2 += String(hour) + ":";
    }


    if (minute < 10) {
      time2 = time2 + "0" + String(minute) + ":";
    } else {
      time2 += String(minute) + ":";
    }


    if (second <= 9) {
      time2 += "0" + String(second);
    }

    else {
      time2 += String(second);
    }

  } else
    time2 = "00:00:00";

  time = time2;
}

void prepareGpsLat() {

  if (gps.location.isValid()) {
    gpsLat = String(gps.location.lat(), 6);
  } else {
    gpsLat = "00.000000";
  }
}

void prepareGpsLong() {

  if (gps.location.isValid()) {
    gpsLong = String(gps.location.lng(), 6);
  } else {
    gpsLong = "00.000000";
  }
}


void prepareAltitude() {
  float measure = gps.altitude.meters();
  altitude = String(measure);
}

void prepareSpeed() {
  float measure = gps.speed.kmph();
  speed = String(measure);
}


void sendData(char header, const String& data) {
  String packet = String(header) + data;

  if (radio.write(packet.c_str(), packet.length() + 1)) {
    Serial.println(data + " sent.");
  } 
  else {
    Serial.println(data + " failed.");
  }
}
