#include <Arduino.h>
#include "BluetoothSerial.h" // We will include the Serial Bluetooth header
#include <TFT_eSPI.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
#define LED 26
BluetoothSerial BT; // Bluetooth Object
TFT_eSPI tft = TFT_eSPI();
String GetLine(void);


bool toggle = true;
String RxBuffer = "";
char RxByte;

void setup() {
  Serial.begin(115200); // Initializing serial connection for debugging
  BT.begin("ESP32_LED_Control"); // Name of your Bluetooth Device and in slave mode
  Serial.println("Bluetooth Device is Ready to Pair");
  pinMode (LED, OUTPUT); // Change the LED pin to OUTPUT

  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Temperatura", 10, 18, 2);
  tft.drawString("Humedad", 10, 88, 2);
  tft.drawString("`C", 63, 40, 4);
  tft.drawString("%", 63, 110, 4);
}
void loop() {
   if (BT.available()){
    String S=GetLine();

    const int length = S.length();
    char* char_array = new char [length + 1];
    strcpy(char_array, S.c_str());
    String t = String(char_array[0])+String(char_array[1])+String(char_array[2])+String(char_array[3]);
    String h = String(char_array[4])+String(char_array[5])+String(char_array[6])+String(char_array[7]);


    Serial.print("Received: ");
    Serial.println(t);
    Serial.println(h);
    Serial.println(S);


    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString(t, 10, 40, 4);
    tft.drawString(h, 10, 110, 4);
    BT.flush();

    delete[] char_array;
}
}

String GetLine(void)
  {
      String S = "";
      if (BT.available()){
        char c = BT.read();
        while (c != '\n'){
          S = S + c;
          delay(1);
          c = BT.read();
        }
      }
    return(S + '\n');
  }
