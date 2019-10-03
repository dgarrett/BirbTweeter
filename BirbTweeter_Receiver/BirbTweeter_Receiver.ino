#include "heltec.h" 
#include "Adafruit_Thermal.h"

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --"; // Will store signal strength
String packSize = "--";
String packet;

// Set to 0 if you aren't connecting a printer
#define PRINTER 1

// Use thermal printer on Serial port 2 with DTR pin 13
// DTR is used for the printer to tell us when it's ready
// for more data
Adafruit_Thermal printer(&Serial2, 13);

void HeltecLoRaData(){
  // Reset the OLED display
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);

  // Print size of packet and signal strength on OLED
  Heltec.display->drawString(0 , 0 , "Got "+ packSize + " B, " + rssi);
  // Print message on OLED
  Heltec.display->drawStringMaxWidth(0 , 11 , 130, packet);
  Heltec.display->display();
#if PRINTER
  // Print message on thermal printer
  printer.println(packet);
  // Feed the thermal paper out a little bit
  printer.feed(1);
#endif
}

void cbk(int packetSize) {
  // Reset our packet variable
  packet ="";
  // Get the size of our received packet
  packSize = String(packetSize,DEC);
  // Read every byte received from LoRa
  for (int i = 0; i < packetSize; i++) { packet += (char) Heltec.LoRa.read(); }
  // Store the signal strength as a string to print
  rssi = "RSSI " + String(Heltec.LoRa.packetRssi(), DEC);

  // Call our data handling callback
  HeltecLoRaData();
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  // Setup OLED display
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();

  // Setup thermal printer to print using Serial port 2
#if PRINTER
  Serial2.begin(9600); // Printer
  printer.begin();

  printer.println(F("Printer init success"));
  // Feed the printer paper out a little bit
  printer.feed(2);

  printer.setDefault();
#endif
  
  delay(1000);

  // Turn on LoRa to receive
  Heltec.LoRa.receive();
}

void loop() {
  // Check if we received a LoRa packet, call `cbk()` if we did
  int packetSize = Heltec.LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}
