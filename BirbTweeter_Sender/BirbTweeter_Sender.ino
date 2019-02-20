#include "heltec.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

const char *ssid = "BirbTweeterSender";
//const char *password = "yourPassword";

const char *html_index = R"(
#include "data/index.html"
)";

WiFiServer server(80);

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();

  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, String("AP IP address: ") + String(myIP));
  Heltec.display->display();
  server.begin();
  
  delay(1000);
}

void oldloop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  // send packet
  Heltec.LoRa.beginPacket();
  Heltec.LoRa.print("Hello there ");
  Heltec.LoRa.print(counter);
  Heltec.LoRa.endPacket();

  counter++;
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

int contentlen = 0;

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    bool headersDone = false;
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c != '\r' && c != '\n') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (headersDone) {
          contentlen--;
          //Serial.println(String("\n>>>contentlen: ") + contentlen);
        }
        if (c == '\n' || (headersDone && contentlen <= 0)) {                    // if the byte is a newline character

          if (currentLine.startsWith("Content-Length: ")) {
            String len = currentLine.substring(sizeof("Content-Length: ")-1);
            //Serial.println(String("mylen: ") + len);
            contentlen = atoi(len.c_str());
          }
          if (currentLine.startsWith("tosend=")) {
            String tosend = currentLine.substring(sizeof("tosend=")-1);
            Serial.println(String("\ntosend: ") + tosend);

            Heltec.LoRa.beginPacket();
            Heltec.LoRa.print(tosend);
            Heltec.LoRa.endPacket();
          }
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            headersDone = true;
          }
          
          if (headersDone && contentlen <= 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            /*client.print("Click <a href=\"/H\">here</a> to turn ON the LED.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn OFF the LED.<br>");
            client.print("<form action=\"/send\" method=\"post\">");
            client.print("To Send: <input type=\"text\" name=\"tosend\"><br>");
            client.print("<input type=\"submit\" value=\"Submit\">");
            client.print("</form>");*/
            client.print(html_index);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
          
        }

        

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          //digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          //digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
