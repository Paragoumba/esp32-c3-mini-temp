#include "DHT.h"
#include <U8g2lib.h>
#define OLED_RESET U8X8_PIN_NONE  // Reset pin
#define OLED_SDA 5
#define OLED_SCL 6
#define LED_BUILTIN 8

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_SCL, OLED_SDA);
int width = 72;
int height = 40;
int xOffset = 30;  // = (132-w)/2
int yOffset = 12;  // = (64-h)/2

#define DHT_PIN 0
#define DHT_TYPE DHT22

DHT dht22(DHT_PIN, DHT_TYPE);

int c = 0;

void handle_oled(char* f, float t, float h) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawStr(xOffset + 0, yOffset + 10, "Display is working!");
  u8g2.drawStr(xOffset + 0, yOffset + 20, "Have fun with it");
  char buffer[20];
  snprintf(buffer, sizeof(buffer), f, t, h);
  u8g2.drawStr(xOffset + 0, yOffset + 30, buffer);
  u8g2.sendBuffer();
}

void setup(void) {
  Serial.begin(9600);
  dht22.begin();
  u8g2.begin();
  u8g2.setContrast(255);     // set contrast to maximum
  u8g2.setBusClock(400000);  //400kHz I2C
  Serial.println(String("LED_BUILTIN ") + LED_BUILTIN);
  Serial.println(String() + u8g2.getDisplayWidth() + "x" + u8g2.getDisplayHeight());
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop(void) {
  float humi = dht22.readHumidity();
  // read temperature as Celsius
  float temperature_C = dht22.readTemperature();
  // read temperature as Fahrenheit
  float temperature_F = dht22.readTemperature(true);

  // check if any reads failed
  if (isnan(humi) || isnan(temperature_C) || isnan(temperature_F)) {
    handle_oled("Could not read\nsensor! %.1f %.1f", c, 0);
    Serial.printf("Could not read sensor! %.1f %.1f\n", c, humi);
  } else {
    handle_oled("Temp is %.1fC and\nhumi is %.1f%%", temperature_C, humi);
    Serial.printf("humi=%.2f,temp=%.2f%\n", humi, temperature_C);
  }
  c++;
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}