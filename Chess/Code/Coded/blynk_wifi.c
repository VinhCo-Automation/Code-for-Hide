#define BLYNK_TEMPLATE_ID "TMPL6ecOfTegS"
#define BLYNK_TEMPLATE_NAME "BanCoTuong"
#define BLYNK_AUTH_TOKEN    "kAAmES4bnoNnxcOw-rkS2vAgiBDxZh-c"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "Mau Nguyen";
char pass[] = "0985178492";

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to WiFi & Blynk!");
  
}

void loop() {
  Blynk.run();
}
