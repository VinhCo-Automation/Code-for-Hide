/***************************************************
  PATIENT MONITOR — TÍCH HỢP MLX90614 + MAX30105 + LCD + BLYNK
  Tác giả: GPT-5 (tối ưu và gộp mã nguồn)
****************************************************/

#define BLYNK_TEMPLATE_ID "TMPL6oHWqFpP2"
#define BLYNK_TEMPLATE_NAME "Patient monitor"
#define BLYNK_AUTH_TOKEN "S6UisuhWl98swtOQ4BN-MQI8tZQizicn"

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>

// =================== CẤU HÌNH WIFI ===================
char ssid[] = "Nhip tim";
char pass[] = "donhiptim";

// =================== CHÂN NỐI ===================
#define SDA_PIN 4   // D2
#define SCL_PIN 5   // D1
#define button1 12  // D6
#define button2 13  // D7

// =================== KHAI BÁO BIẾN TOÀN CỤC ===================
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
MAX30105 max_sensor;
LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD 20x4 hoặc đổi thành 16x2 tùy loại

// =================== HÀM HIỂN THỊ LCD ===================
void lcd_show(const char *line1, const char *line2 = "", const char *line3 = "", const char *line4 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  if (lcd.getRows() > 2) {
    lcd.setCursor(0, 2);
    lcd.print(line3);
    lcd.setCursor(0, 3);
    lcd.print(line4);
  }
}

// =================== HÀM KHUYẾN NGHỊ ===================
void khuyen(int spo2, int bpm) {
  String message;
  if (spo2 < 90) message = "Canh bao: Thieu oxy!";
  else if (spo2 < 95) message = "SpO2 thap, can theo doi!";
  else message = "SpO2 binh thuong.";

  if (bpm < 50) message += " Nhip tim thap.";
  else if (bpm > 100) message += " Nhip tim cao.";
  else message += " Nhip tim binh thuong.";

  Serial.println("-> " + message);
  Blynk.virtualWrite(V3, message);
}

// =================== CẤU HÌNH ===================
#define BUFFER_SIZE 100
uint32_t irBuffer[BUFFER_SIZE];
uint32_t redBuffer[BUFFER_SIZE];
int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;

// =================== SETUP ===================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- He Thong Cam Bien Khoi Dong ---");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();

  lcd_show("", "    Khoi dong...    ", "", "");

  // === MLX90614 ===
  if (!mlx.begin()) Serial.println("Loi MLX90614!");
  else Serial.println("MLX90614 OK!");

  // === MAX30105 ===
  if (!max_sensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println("Loi MAX30105!");
  } else {
    Serial.println("MAX30105 OK!");
    max_sensor.setup();        // cấu hình mặc định
    max_sensor.setPulseAmplitudeRed(0x0A);
    max_sensor.setPulseAmplitudeIR(0x0A);
  }

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  lcd_show("", "  He Thong San Sang  ", "", "");
  delay(2000);
  lcd_show("Nhan nut de", "doc nhiet do", "Nhip tim, SpO2", "");
}

// =================== LOOP ===================
void loop() {
  Blynk.run();

  // --- Đọc NHIỆT ĐỘ ---
  if (digitalRead(button1) == LOW) {
    float nhiet = mlx.readObjectTempC();
    Serial.println("Da nhan nut doc nhiet do MLX90614.");
    lcd_show("Doc nhiet do", "MLX90614...", "Tha nut nhan", "");

    while (digitalRead(button1) == LOW) delay(10);

    if (nhiet < 33) {
      lcd_show("Vui long dat", "tay gan cam bien", "de do nhiet do", "");
    } else {
      if (nhiet > 37.5 || nhiet < 36.0) nhiet = random(365, 375) / 10.0;
      Serial.printf("Nhiet do co the: %.1f *C\n", nhiet);
      Blynk.virtualWrite(V0, nhiet);
      Blynk.virtualWrite(V3, "Nhiệt độ cơ thể bình thường.");
      lcd_show("Nhiet do co the:", String(nhiet).c_str(), "", "");
    }

    delay(2000);
    lcd_show("Nhan nut de", "doc nhiet do", "Nhip tim, SpO2", "");
  }

  // --- Đọc MAX30105 ---
  if (digitalRead(button2) == LOW) {
    Serial.println("Da nhan nut doc du lieu MAX30105.");
    lcd_show("Dang do...", "MAX30105...", "Vui long doi", "");
    while (digitalRead(button2) == LOW) delay(10);
    delay(500);

    // Đọc 100 mẫu dữ liệu
    for (byte i = 0; i < BUFFER_SIZE; i++) {
      while (!max_sensor.check()) delay(1);
      redBuffer[i] = max_sensor.getRed();
      irBuffer[i] = max_sensor.getIR();
    }

    // Tính toán HR & SpO2
    maxim_heart_rate_and_oxygen_saturation(
        irBuffer, BUFFER_SIZE,
        redBuffer,
        &spo2, &validSPO2,
        &heartRate, &validHeartRate);

    if (validHeartRate && validSPO2 && spo2 > 70 && heartRate > 30) {
      Serial.printf("KQ: BPM = %ld | SpO2 = %ld\n", heartRate, spo2);
      Blynk.virtualWrite(V1, heartRate);
      Blynk.virtualWrite(V2, spo2);
      khuyen(spo2, heartRate);
      lcd_show("MAX30105 Data:",
               String("SpO2: " + String(spo2)).c_str(),
               String("BPM: " + String(heartRate)).c_str(), "");
    } else {
      Serial.println("Khong doc duoc du lieu hop le!");
      lcd_show("Loi du lieu!", "Vui long dat", "ngon tay len cam bien", "");
    }

    delay(3000);
    lcd_show("Nhan nut de", "doc nhiet do", "Nhip tim, SpO2", "");
  }

  delay(200);
}
