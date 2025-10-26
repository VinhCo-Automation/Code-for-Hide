#define BLYNK_TEMPLATE_ID "TMPL6ecOfTegS"
#define BLYNK_TEMPLATE_NAME "BanCoTuong"
#define BLYNK_AUTH_TOKEN    "kAAmES4bnoNnxcOw-rkS2vAgiBDxZh-c"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "calculate.h"
#include "in_out.h"
#include "step.h"
#include "lcd_display.h"
int x_nhan;
int y_nhan;
int x_danh;
int y_danh;
int button_blynk;
int reset;
int position1;
int position2;
int frequency1 = 2000;
int frequency2 = 2000;
int frequency3 = 2000;

char ssid[] = "Vinh";
char pass[] = "21138008";

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to WiFi & Blynk!");
  setup_input_pins();
  setup_output_pins();
  lcd_init();
  lcd_show("   Chessboard   ","     Ready!     ");
  home_all();
  Serial.println("All axes are at home position.");
  delay(1000);
  position1 = 0;
  position2 = 0;
  lcd_show("Position X:" + String(position1).c_str(), "Position Y:" + String(position2).c_str());

}

BLYNK_WRITE(V0) {
  x_nhan = param.asInt();
}
BLYNK_WRITE(V1) {
  y_nhan = param.asInt();
}
BLYNK_WRITE(V3) {
  x_danh = param.asInt();
}
BLYNK_WRITE(V4) {
  y_danh = param.asInt();
}
BLYNK_WRITE(V2) {
  int button = param.asInt();
  if(button == 1){
    button_blynk = 1;
  }
}
BLYNK_WRITE(V5) {
  int a = param.asInt();
  reset = a;
  if(reset == 1){
    home_all();
    position1 = 0;
    position2 = 0;
    lcd_show("Position X:" + String(position1).c_str(), "Position Y:" + String(position2).c_str());
    coi(1, 1000);
    button_blynk = 0;
    reset = 0;
  }
}

void loop(){
  Blynk.run();

  if (button_blynk == 1){
    if(x_nhan == x_danh && y_nhan == y_danh){
      Serial.println("Toa do nhan va toa do danh khong duoc trung nhau");
    }else{
      int pulse1 = pulse(x_nhan, position1);
      int pulse2 = pulse(y_nhan, position2);
      int pulse3 = pulse(x_danh, position3);
      int pulse4 = pulse(y_danh, position4);
      move_axis1(pulse1);
      move_axis2(pulse2);
      position1 = x_nhan;
      position2 = y_nhan;
      lcd_show("Position X:" + String(position1).c_str(), "Position Y:" + String(position2).c_str());
      delay(1000);
      pink();
      move_axis3(pulse3);
      move_axis4(pulse4);
      position3 = x_danh;
      position4 = y_danh;
      lcd_show("Position X:" + String(position3).c_str(), "Position Y:" + String(position4).c_str());
      delay(1000);
      drop();
      coi(2, 1000);
    }
    button_blynk = 0;
  }
  delay(1000);
}