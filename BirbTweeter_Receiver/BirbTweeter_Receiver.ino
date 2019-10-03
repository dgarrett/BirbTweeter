#include "heltec.h" 
#include "Adafruit_Thermal.h"

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
String rssi = "RSSI --";
String packSize = "--";
String packet ;

// Set to 0 if you aren't connecting a printer
#define PRINTER 1

Adafruit_Thermal printer(&Serial2, 13);

void HeltecLoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0 , 0 , "Got "+ packSize + " B, " + rssi);
  Heltec.display->drawStringMaxWidth(0 , 11 , 130, packet);
  Heltec.display->display();
#if PRINTER
  printer.println(packet);
  printer.feed(1);
#endif
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) Heltec.LoRa.read(); }
  rssi = "RSSI " + String(Heltec.LoRa.packetRssi(), DEC) ;
  HeltecLoRaData();
}

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();

#if PRINTER
  Serial2.begin(9600); // Printer
  printer.begin();

  printer.println(F("Printer init success"));
  printer.feed(2);

  printer.setDefault();
#endif
  
  delay(1000);
  //Heltec.LoRa.onReceive(cbk);
  Heltec.LoRa.receive();
}

void loop() {
  int packetSize = Heltec.LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}
