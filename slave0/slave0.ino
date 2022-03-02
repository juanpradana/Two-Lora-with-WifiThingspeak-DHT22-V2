#include "DHT.h"

#define Sender_Txd_pin 17
#define Sender_Rxd_pin 16
#define DHTPIN 4

#define DHTTYPE DHT22

HardwareSerial Receiver(1);
DHT dht(DHTPIN, DHTTYPE);

float temperatureC;

void setup() {
  Serial.begin(9600);
  Receiver.begin(9600, SERIAL_8N1, Sender_Rxd_pin, Sender_Txd_pin);
  dht.begin();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.println("User#0: " + String(h)+ ";" + String(t) + ";" + String(hic));
  Receiver.print("User#0: " + String(h)+ ";" + String(t) + ";" + String(hic) + "%$");
  delay(5000);
}
