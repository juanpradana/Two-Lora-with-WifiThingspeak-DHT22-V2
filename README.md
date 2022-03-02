# Two-Lora-with-WifiThingspeak-DHT22-V2
This repository for one ESP32 as gateway where it will send all data to Thingspeak and two ESP32 as sender/station using Lora e32-433t30d.

This Repository is other methode and more efficient code write.

![Lora e32-433t30d](https://www.ebyte.com/Uploadfiles/Picture/2021-5-25/20215251730359457.jpg)

# Wiring
## LoRa
ESP32 >>  Lora e32-433t30d

GND   >> GND

GND   >> M0

GND   >> M1

D16   >> Tx

D17   >> Rx

3.3V  >> VCC

## DHT22
ESP32 >> DHT22

3.3V >> Vcc

GND >> GND

D4 >> DATA

# Explanation
slave0 is identified as User#1 and slave1 i identified as User#2, data sended to gateway every 5 Second.

CountOut is used for counting failed queue data from station, when countOut is 3 so queue will change to other station (station 1 to station 0). CountOut 0 for slave0 and countOut 1 for slave1.

Time delay set to 20 seconds cause free ThingSpeak just can received new data for every 15 seconds.
