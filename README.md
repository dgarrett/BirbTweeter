# BirbTweeter
Send messages over LoRa to a thermal printer

## What you need
- 2x ESP32 Development Boards with LoRa (ex https://smile.amazon.com/gp/product/B076MTF8NW/)
  - One will be used as the Sender
  - One will be used as the receiver
- Serial Thermal Printer and paper (ex https://www.adafruit.com/product/597)

## Wiring Instructions for Sender ESP32 board
Just plug into USB for power

## Wiring Instructions for Receiver ESP32 board
One of your boards will be the receiver, so it needs to be connected to the thermal printer.

Follow Adafruit directions for connecting a thermal printer: https://learn.adafruit.com/mini-thermal-receipt-printer/microcontroller

- Red -> 5V
- Black -> GND
- Blue -> Pin 17 (U2TXD)
- Green -> Pin 16 (U2RXD)
- If your thermal printer has an extra DTR connection (Yellow), connect it to pin 13. Otherwise, modify the `Adafruit_Thermal printer(&Serial2, 13);` line in `BirbTweeter_Receiver.ino` and change it to `Adafruit_Thermal printer(&Serial2);`

## Software Setup
1. Set up the Heltec ESP32 Arduino Core: https://docs.heltec.cn/#/en/user_manual/how_to_install_esp32_Arduino
2. In the Arduino IDE, select Tools -> Manage Libraries and install the library: Adafruit Thermal Printer Library
3. Deploy BirbTweeter_Sender to one of the ESP32 boards
4. Deploy BirbTweeter_Receiver to the other board that is connected to the thermal printer
5. Power on both boards.
6. On a computer or phone, connect to the "BirbTweeterSender" WiFi hotspot that is hosted by the Sender
7. Open a web browser and navigate to 192.168.4.1
8. Enter a message and click Send
9. The Sender will send the message to the Receiver over LoRa and the Receiver will print it.

Note that there is currently no retry, so the message may fail to be delivered.
