#include <heltec.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <Adafruit_Thermal.h>

#include "credentials.h"

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6

const char *html_root =
#include "index.html.h"
;

WebServer server(80);

// Set to 0 if you aren't connecting a printer
#define PRINTER 0

// Use thermal printer on Serial port 2 with DTR pin 13
// DTR is used for the printer to tell us when it's ready
// for more data
Adafruit_Thermal printer(&Serial2, 13);

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, String("IP address: ") + WiFi.localIP());
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

  // Send our HTML when the user navigates to our IP address
  server.on("/", []() {
    Serial.println("hello");
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

    Serial.println(String("tosend: ") + String(server.arg("tosend")));

#if PRINTER
    // Print message on thermal printer
    printer.println(server.arg("currenttime"));
    printer.println(server.arg("tosend"));
    // Feed the thermal paper out a little bit
    printer.feed(1);
#endif
  });

  // Start server on WiFi with the above configuration
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  // Let the server library process requests
  server.handleClient();
}
