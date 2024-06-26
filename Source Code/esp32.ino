#include <SPI.h>
#include <RF24.h>

#define CE_PIN 22
#define CSN_PIN 21
const byte thisSlaveAddress[6] = "00001";
RF24 radio(CE_PIN, CSN_PIN);

// Define maximum length for the character array
const int MAX_DATA_LENGTH = 32;

String receivedData;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setChannel(6);
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1, thisSlaveAddress);
  radio.startListening();
}

void loop() {

  if (radio.isChipConnected()) {
    
    if (radio.available()) {

      receiveData();
      processReceivedData();
    }
  } else {
    Serial.println("NRF24 module is not connected. Board restarting.");
    delay(100);
  }
}

void receiveData() {
  // Receive data
  char data[MAX_DATA_LENGTH];
  radio.read(&data, sizeof(data));
  receivedData = String(data);
  receivedData.trim();  // Remove any leading or trailing whitespace
}

void processReceivedData() {
  // Check the header and process accordingly
  char header = receivedData.charAt(0);
  String payload = receivedData.substring(1);

  switch (header) {
    case 'D':
      {
        
        Serial.print("Date: ");
        Serial.println(payload);
        break;
      }

      case 'T':
      {
        
        Serial.print("Time: ");
        Serial.println(payload);
        break;
      }

      case 'C':
    {
      Serial.print("Temperature: ");
      Serial.println(payload + " C");
      break;
    }

    case 'H':
    {
      Serial.print("Humidity: ");
      Serial.println(payload + " %");
      break;
    }

    case 'P':
    {
      Serial.print("Pressure: ");
      Serial.println(payload + " hPa");
      break;
    }

    case 'L':
    {
      Serial.print("Latitude: ");
      Serial.println(payload);
      break;
    }

    case 'G':
    {
      Serial.print("Longitude: ");
      Serial.println(payload);
      break;
    }

     case 'A':
      {
        Serial.print("Altitude: ");
        Serial.println(payload + " m");
        break;
      }

      case 'S':
      {
        Serial.print("Speed: ");
        Serial.println(payload + " km/h");
        break;
      }

      case 'Ε':
      {
        Serial.print("Εrror: ");
        Serial.println(payload);
        break;
      }
      
    default:
      Serial.println(payload);
      break;
  }
}
