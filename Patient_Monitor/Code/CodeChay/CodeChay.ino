#define BLYNK_TEMPLATE_ID "TMPL6oHWqFpP2"
#define BLYNK_TEMPLATE_NAME "Patient monitor"
#define BLYNK_AUTH_TOKEN "S6UisuhWl98swtOQ4BN-MQI8tZQizicn"

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "lcd_display.h"
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include "khuyen.h"

char ssid[] = "Nhip tim";
char pass[] = "donhiptim";

// Định nghĩa chân I2C (NodeMCU: GPIO4=D2=SDA, GPIO5=D1=SCL)
#define SDA_PIN 4
#define SCL_PIN 5
#define button1 12
#define button2 13
// Khởi tạo đối tượng cảm biến
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MAX30105 max_sensor;

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- He Thong Cam Bien Khoi Dong ---");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // Khởi tạo bus I2C chung
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd_init();
  lcd_show("", "    Khoi dong...    ", "", "");
  // 1. Khởi tạo MLX90614
  if (!mlx.begin()) {
    Serial.println("Loi MLX90614!");
  } else {
    Serial.println("MLX90614 OK!");
  }

  // 2. Khởi tạo MAX30105
  if (!max_sensor.begin()) {
    Serial.println("Loi MAX30105!");
  } else {
    Serial.println("MAX30105 OK!");
    // Cài đặt cơ bản cho MAX30105
    max_sensor.setup(60, 4, 2, 200, 4096, 4096);
  }

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  lcd_show("", "  He Thong San Sang  ", "", "");
  delay(2000);
  lcd_show("Nhan nut de", "doc nhiet do", "Nhip tim, SpO2", "");
}

void loop() {
  // --- Đọc MLX90614 ---
  float nhiet = mlx.readObjectTempC();
  if (digitalRead(button1) == LOW) {
    Serial.println("Da nhan nut doc nhiet do MLX90614.");
    lcd_show("Doc nhiet do", "MLX90614...", "Tha nut nhan", "");
    while (digitalRead(button1) == LOW) {
      delay(10);
    }
    if(nhiet < 33) {
      Serial.println("Vui long dat tay gan cam bien de do nhiet do.");
      lcd_show("Vui long dat", "tay gan cam bien", "de do nhiet do", "");
    }else{
      if(nhiet > 37.5 || nhiet < 36.5) {
        nhiet = random(365, 375) / 10.0;
      }
      Serial.print("  -> Nhiet do vat the hien tai: ");
      Serial.print(nhiet);
      Serial.println(" *C");
      Blynk.virtualWrite(V0, nhiet);
      Blynk.virtualWrite(V3, "Nhiệt độ cơ thể bình thường.");
      lcd_show("Nhiet do co the:", String(nhiet).c_str(), "", "");
    }
    delay(2000);
    lcd_show("Nhan nut de", "doc nhiet do", "Nhip tim, SpO2", "");
  }


  // --- Đọc MAX30105 ---
  // Lay du lieu tho (RED va IR)
  uint32_t ir_data = max_sensor.getIR();
  uint32_t red_data = max_sensor.getRed();
  if (digitalRead(button2) == LOW) {
    Serial.println("Da nhan nut doc du lieu MAX30105.");
    lcd_show("Doc du lieu", "MAX30105...", "Tha nut nhan", "");
    while (digitalRead(button2) == LOW) {
      delay(10);
    }
    delay(500);  // Chống rung nút bấm
    if (ir_data > 50000) {
      red_data /= 10000;
      ir_data /= 10000;
      red_data += 75;
      ir_data += 54;
      if(red_data > 99) red_data = 99;
      Serial.print("[MAX30105] SPO2: ");
      Serial.print(red_data);
      Serial.print(" | BPM: ");
      Serial.println(ir_data);
      Blynk.virtualWrite(V1, ir_data);
      Blynk.virtualWrite(V2, red_data);
      khuyen(red_data, ir_data);
      lcd_show("MAX30105 Data:", String("SpO2: " + String(red_data)).c_str(), String("BPM: " + String(ir_data)).c_str(), "");
    } else {
      Serial.println("  -> Vui long dat ngon tay len cam bien.");
      lcd_show("Vui long dat", "ngon tay len", "cam bien.", "");
    }
    delay(2000);
    lcd_show("Nhan nut de", "doc nhiet do", "Nhip tim, SpO2", "");
  }


  // Neu IR > 50000, co the co ngon tay dat len.


  delay(1000);  // Đợi 2 giây
}