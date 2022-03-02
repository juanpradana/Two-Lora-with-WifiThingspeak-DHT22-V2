#include <WiFi.h>
#include "ThingSpeak.h"
#include "time.h"

HardwareSerial Sender(1);

#define Sender_Txd_pin 17
#define Sender_Rxd_pin 16

const char* ssid = "YOUR_SSID";   // your network SSID (name) 
const char* password = "YOUR_PASSWORD";   // your network password

WiFiClient  client;

const char* ntpServer = "pool.ntp.org";

unsigned long tempTime;

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

const char* server = "api.thingspeak.com";
String myWriteAPIKey = "YOUR_WRITE_API_KEY";

// Variable to hold temperature readings
String fullDatas;
String humiFloat;
String temperatureC;
String hicFloat;
int queue = 0;
int countOut = 0;

void setup() {
  Serial.begin(9600);
  Sender.begin(9600, SERIAL_8N1, Sender_Rxd_pin, Sender_Txd_pin);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  configTime(25200, 0, ntpServer);
  tempTime = getTime();
  Serial.println(tempTime);
}

void loop() {
  if (Sender.available()) {
    String datas = Sender.readString();
    Serial.println(datas);
    unsigned long timeNow = getTime();
    Serial.println(timeNow - tempTime);
    if (timeNow - tempTime > 20) {
      Serial.println("20s");
      countOut++;
      if (countOut >= 3) {
        if (queue == 0) {
          queue++;
        }
        else {
          queue = 0;
        }
      }
      if (queue == 0 && datas.indexOf("#0") > 0) {
        int nilaiIndex = datas.indexOf("User#0: ");
        String dataCapture = datas.substring(nilaiIndex, (nilaiIndex + 27));
        String finalDatas = getValue(dataCapture, '%', 0);
        finalDatas.replace("User#0: ", "");
        Serial.println(finalDatas);
        Serial.println("start 0");
        humiFloat = getValue(finalDatas, ';', 0);
        temperatureC = getValue(finalDatas, ';', 1);
        hicFloat = getValue(finalDatas, ';', 2);
        if (client.connect(server, 80))
        {
          String postStr = "/update?";
          postStr += "key=";
          postStr += myWriteAPIKey;
          postStr += "&field1=";
          postStr += temperatureC;
          postStr += "&field3=";
          postStr += humiFloat;
          postStr += "&field5=";
          postStr += hicFloat;
      
          client.print(String("GET ") + postStr + " HTTP/1.1\r\n" +
                   "Host: " + server + "\r\n" +
                   "Connection: close\r\n\r\n");
          unsigned long timeout = millis();
          while (client.available() == 0) {
            if (millis() - timeout > 5000) {
              Serial.println(">>> Client Timeout !");
              client.stop();
              return;
            }
          }
      
          while (client.available()) {
            String line = client.readStringUntil('\r');
            Serial.print(line);
          }
      
          Serial.println();
          Serial.println("Success slave0");
        }
        queue++;
        countOut = 0;
      }
      else if (queue == 1 && datas.indexOf("#1") > 0) {
        int nilaiIndex = datas.indexOf("User#1: ");
        String dataCapture = datas.substring(nilaiIndex, (nilaiIndex + 27));
        String finalDatas = getValue(dataCapture, '%', 0);
        finalDatas.replace("User#1: ", "");
        Serial.println(finalDatas);
        Serial.println("start 1");
        humiFloat = getValue(finalDatas, ';', 0);
        temperatureC = getValue(finalDatas, ';', 1);
        hicFloat = getValue(finalDatas, ';', 2);
        if (client.connect(server, 80))
        {
          String postStr = "/update?";
          postStr += "key=";
          postStr += myWriteAPIKey;
          postStr += "&field2=";
          postStr += temperatureC;
          postStr += "&field4=";
          postStr += humiFloat;
          postStr += "&field6=";
          postStr += hicFloat;
      
          client.print(String("GET ") + postStr + " HTTP/1.1\r\n" +
                   "Host: " + server + "\r\n" +
                   "Connection: close\r\n\r\n");
          unsigned long timeout = millis();
          while (client.available() == 0) {
            if (millis() - timeout > 5000) {
              Serial.println(">>> Client Timeout !");
              client.stop();
              return;
            }
          }
      
          while (client.available()) {
            String line = client.readStringUntil('\r');
            Serial.print(line);
          }
      
          Serial.println();
          Serial.println("Success slave1");
        }
        queue = 0;
        countOut = 0;
      }
      tempTime = timeNow; 
    }
    Serial.println(queue);
  }
  delay(2000);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
 
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
