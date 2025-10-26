#include "in_out.h"

void run_step(int pulse, int pul){
    pulse = abs(pulse);
    for (int i = 0; i < pulse; i++)
    {
        Serial.print("Pulse: ");
        Serial.println(i);
    }
}
void move_axis1(int pulse){
    if (pulse > 0){
        Serial.print("Dir1: HIGH   ");
        Serial.print("Move Axis1 to: ");
        Serial.println(position1 + pulse);
    }else{
        Serial.println("Dir1: LOW");
        Serial.print("Move Axis1 to: ");
        Serial.println(position1 + pulse);
    }
    position1 += pulse;
}
void move_axis2(int pulse){
    if (pulse > 0){
        Serial.print("Dir2: HIGH   ");
        Serial.print("Move Axis2 to: ");
        Serial.println(position2 + pulse);
    }else{
        Serial.println("Dir2: LOW");
        Serial.print("Move Axis2 to: ");
        Serial.println(position2 + pulse);
    }
}
void move_axis3(int status){
    if (status == 1){
        Serial.println("Dir3: HIGH");
        Serial.println("Move Axis3 to: 1600");
    }else{
        Serial.println("Dir3: LOW");
        Serial.println("Move Axis3 to: 0");
    }
}

void pick(){
    delay(500);
    move_axis3(1);
    delay(500);
    digirtalWrite(relay, HIGH);
    delay(500);
    move_axis3(0);
    delay(500);
}

void drop(){
    delay(500);
    move_axis3(1);
    delay(500);
    digitalWrite(relay, LOW);
    delay(500);
    move_axis3(0);
    delay(500);
}
