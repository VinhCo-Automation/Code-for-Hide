#include "in_out.h"
void home_axis1(int frequency1){
    digitalWrite(dir1, LOW);
    while (digitalRead(s1) == LOW)
    {
        digitalWrite(pul1, HIGH);
        delayMicroseconds(frequency1);
        digitalWrite(pul1, LOW);
        delayMicroseconds(frequency1);
    }
}
void home_axis2(int frequency2){
    digitalWrite(dir2, HIGH);
    while (digitalRead(s3) == LOW)
    {
        digitalWrite(pul2, HIGH);
        delayMicroseconds(frequency2);
        digitalWrite(pul2, LOW);
        delayMicroseconds(frequency2);
    }
}
void home_axis3(int frequency3){
    digitalWrite(dir3, LOW);
    while (digitalRead(s5) == LOW)
    {
        digitalWrite(pul3, HIGH);
        delayMicroseconds(frequency3);
        digitalWrite(pul3, LOW);
        delayMicroseconds(frequency3);
    }
}
void home_all(){
    home_axis1();
    home_axis2();
    home_axis3();
}
void run_step(int pulse, int pul){
    pulse = abs(pulse);
    for (int i = 0; i < pulse; i++)
    {
        digitalWrite(pul, HIGH);
        delayMicroseconds(frequency);
        digitalWrite(pul, LOW);
        delayMicroseconds(frequency);
    }
}
void move_axis1(int pulse){
    if (pulse > 0){
        digitalWrite(dir1, HIGH);
        run_step(pulse, pul1);
    }else{
        digitalWrite(dir1, LOW);
        run_step(pulse, pul1);
    }
    currentPos1 += pulse;
}
void move_axis2(int pulse){
    if (pulse > 0){
        digitalWrite(dir2, HIGH);
        run_step(pulse, pul2);
    }else{
        digitalWrite(dir2, LOW);
        run_step(pulse, pul2);
    }
    currentPos2 += pulse;
}
void move_axis3(int status){
    if (status == 1){
        digitalWrite(dir3, HIGH);
        run_step(1600, pul3);
    }else{
        digitalWrite(dir3, LOW);
        run_step(1600, pul3);
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
void coi(int n, int delay_time){
    for(int i=0; i<n; i++){
        digitalWrite(coil, HIGH);
        delay(delay_time);
        digitalWrite(coil, LOW);
        if(i < n - 1){
            delay(delay_time);
        }
    }
    delay(1000);
}