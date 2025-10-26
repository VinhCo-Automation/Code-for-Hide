#pragma once

// ===================== Input Pins =====================
#define s1 34
#define s2 35
#define s3 39

void setup_input_pins() {
    pinMode(s1, INPUT);
    pinMode(s2, INPUT);
    pinMode(s3, INPUT);
}


// ===================== Output Pins =====================
#define pul1 12
#define pul2 14
#define pul3 27
#define dir1 26
#define dir2 25
#define dir3 33
#define relay 32
#define siren 4

// GPIO 21 is used for I2C SDA
// GPIO 22 is used for I2C SCL

void setup_output_pins() {
    pinMode(pul1, OUTPUT);
    pinMode(pul2, OUTPUT);
    pinMode(pul3, OUTPUT);
    pinMode(dir1, OUTPUT);
    pinMode(dir2, OUTPUT);
    pinMode(dir3, OUTPUT);
    pinMode(relay, OUTPUT);
    pinMode(siren, OUTPUT);
}