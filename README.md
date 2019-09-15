# BirbTweeter
Send messages over LoRa to a thermal printer

## What you need
- 2x ESP32 Development Boards with LoRa (ex https://smile.amazon.com/gp/product/B076MTF8NW/)
- Serial Thermal Printer and paper (ex https://www.adafruit.com/product/597)

## Wiring Instructions for Sender ESP32 board
Just plug into USB for power

## Wiring Instructions for Receiver ESP32 board
One of your boards will be the receiver, so it needs to be connected to the thermal printer.
Follow Adafruit directions for connecting thermal printer: https://learn.adafruit.com/mini-thermal-receipt-printer/microcontroller
If your thermal printer has an extra DTR cable, connect it to pin 13

## Software Setup
1. Set up the Heltec ESP32 Arduino Core: https://docs.heltec.cn/#/en/user_manual/how_to_install_esp32_Arduino
2. In the Arduino IDE, select Tools -> Manage Libraries and install the following libraries: Adafruit Thermal Printer Library
3. Deploy BirbTweeter_Sender to one of the ESP32 boards
4. Deploy BirbTweeter_Receiver to the other board
5. Power on both boards.
6. On a computer or phone, connect to the "BirbTweeterSender" WiFi hotspot that is being run by the Sender
7. Open a web browser and navigate to the IP address shown on the OLED screen of the Sender
8. Enter a message and click Send
