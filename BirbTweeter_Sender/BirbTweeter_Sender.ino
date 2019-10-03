#include "heltec.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6

// WiFi hotspot name
const char *ssid = "BirbTweeterSender";

const char *html_root =
#include "index.html.h"
;

WebServer server(80);

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();

  // Setup WiFi hotspot and print IP on OLED screen
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, String("AP IP address: ") + myIP.toString());
  Heltec.display->display();

  // Send our HTML when the user navigates to our IP address
  server.on("/", []() {
    server.send(200, "text/html", html_root);
  });

  // Our HTML submits the form with a POST to /send
  server.on("/send", HTTP_POST, []() {
    Serial.println("Args: ");
    Serial.println(server.args());
    Serial.println(server.arg("tosend"));
    Serial.println(server.arg("currenttime"));

    // Return the user to our home page
    server.send(200, "text/html", html_root);

    // Display the user's message on our OLED screen
    Heltec.display->drawString(0, 10, String(server.arg("tosend")));
    Heltec.display->display();

    // Create a packet and send the message over LoRa
    Heltec.LoRa.beginPacket();
    Heltec.LoRa.print(server.arg("currenttime") + "\n");
    Heltec.LoRa.print(server.arg("tosend"));
    Heltec.LoRa.endPacket();
  });

  // Start server on WiFi with the above configuration
  server.begin();
}

void loop(void) {
  // Let the server library process requests
  server.handleClient();
}
