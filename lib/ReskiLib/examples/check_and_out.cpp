#include "ReskiLib.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start!");
  // Enable multiplexor 
  pinMode(mx_en, OUTPUT);
  digitalWrite(mx_en, LOW);
  // for every 4 clip move servos and check fill
  for (int i=0;i<4; i++) {
    // select #i clip
    selclip(i);
    setXservo();
    moveservo(X_ang90);
    setZservo();
    moveservo(Z_ang90);
    if (getTCRTtop())
      {
        Serial.print("Full clip #");
        Serial.println(i);
      } else
      {
        Serial.print("Not fill clip #");
        Serial.println(i);
      }
      //delay(1000);
      setZservo();
      moveservo(Z_ang00);
      setXservo();
      moveservo(X_ang00);
  }
  Serial.println("Stop!");
}

void loop() {
  // put your main code here, to run repeatedly:
}