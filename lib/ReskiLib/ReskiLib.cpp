#include "ReskiLib.h"

void moveservo(int angle) {
  pinMode(mx_z, OUTPUT);
  digitalWrite(mx_z, LOW);
  for (int i=1;i<=180;i++){
    digitalWrite(mx_z, HIGH);
    delayMicroseconds(angle);
    digitalWrite(mx_z, LOW);
    delayMicroseconds(20000-angle);
    delay(1);
  }
  digitalWrite(mx_z, LOW);
}
// one clip used 4 channel multiplex CD74HC4067. For work is once - must set pin S2 and S3. This function is made it.
void selclip(int clp){
// set S2 and S3 as 0 and 1 bit in number of clip
    pinMode(mx_s2, OUTPUT);
    pinMode(mx_s3, OUTPUT);
    pinMode(mx_z, OUTPUT);
    digitalWrite(mx_z, LOW);
    digitalWrite(mx_s2,bitRead(clp,0));
	digitalWrite(mx_s3,bitRead(clp,1));
}
// X servo selected S0 = 0, S1 = 1
void setXservo() {
    pinMode(mx_s0, OUTPUT);
    pinMode(mx_s1, OUTPUT);
    pinMode(mx_z, OUTPUT);
    digitalWrite(mx_z, LOW);
    digitalWrite(mx_s0,LOW);
	digitalWrite(mx_s1,HIGH);
}

// Z servo selected S0 = 0, S1 = 0
void setZservo() { // 5 mcs * 3 + 7 mcs * 3 = 45 mcs 
    pinMode(mx_s0, OUTPUT);
    pinMode(mx_s1, OUTPUT);
    pinMode(mx_z, OUTPUT);
    digitalWrite(mx_z, LOW);
    digitalWrite(mx_s0,LOW);
	digitalWrite(mx_s1,LOW);
}

// TCRT5000 on bottom selected & read value S0 = 1, S1 = 0
boolean getTCRTbot() {
    pinMode(mx_s0, OUTPUT);
    pinMode(mx_s1, OUTPUT);
    pinMode(mx_z, INPUT);
    digitalWrite(mx_s0,HIGH);
	digitalWrite(mx_s1,LOW);
    pinMode(mx_z, INPUT);
    return digitalRead(mx_z);
}

// TCRT5000 on top selected & read value S0 = 1, S1 = 1
boolean getTCRTtop() {
    pinMode(mx_s0, OUTPUT);
    pinMode(mx_s1, OUTPUT);
    digitalWrite(mx_s0,HIGH);
	digitalWrite(mx_s1,HIGH);
    pinMode(mx_z, INPUT);
    return digitalRead(mx_z);
}

// ReskiOut- out from selected clip 1 goods
int ReskiOut(int clp) {
    unsigned int i;
    // rc =  0 - Goods out, clip fill - ALL OK!
    // rc = -1 - Clip empty, clip - off 
    // rc = -2 - Goods not out - time out, clip - off
    // rc = -3 - Goods out, clip empty - OK, clip - off
    // select #clp clip
    selclip(clp);
    if (!getTCRTbot()) {
        return 0;  // -1
    } 
    setXservo();
    moveservo(X_ang90);
    setZservo();
    moveservo(Z_ang90);
    i = 0;
    do
    {
        i++;
        if (!getTCRTbot()) break;    
    } while (i < waitout);
    // by way must return servo back
    setZservo();
    moveservo(Z_ang00);
    setXservo();
    moveservo(X_ang00);
    /* Timeout...??? - Oh f..ck */
    if (i == waitout) return 0; // -2
    // wait, while next good down to Z level
    i    = 0;
    do
    {
        i++;
        if (getTCRTbot()) break;    
    } while (i < waitout);
    if (i == waitout) return 0; // -3
    return 0;
}


// ReskiOut- out from selected clip 1 goods
int ReskiOut2(int clp) {
    unsigned int i, j;
    // rc =  0 - Goods out, clip fill - ALL OK!
    // rc = -1 - Clip empty, clip - off 
    // rc = -2 - Goods not out - time out, clip - off
    // rc = -3 - Goods out, clip empty - OK, clip - off
    // select #clp clip
    selclip(clp); 
    if (!getTCRTbot()) {
        return 0; // -1
    } 
    setXservo();
    //for (i=0;i<10;i++) 
    moveservo(X_ang90);
    for (i=1;i<=180;i++){
        // подадим на X servo импульс № i шириной X_angle90
        digitalWrite(mx_s1,HIGH);
        digitalWrite(mx_z, HIGH);
        delayMicroseconds(X_ang90);
        digitalWrite(mx_z, LOW);
        // подадим на Z servo импульс № i шириной Z_angle90
        digitalWrite(mx_s1,LOW);
        digitalWrite(mx_z, HIGH);
        delayMicroseconds(Z_ang90);
        digitalWrite(mx_z, LOW);
        delayMicroseconds(20000 - X_ang90 - Z_ang90);
        delay(1);
    }
    // теперь будем держать Z серво до выпадения коробки или до таймаута
    for (j=1;j<10;j++){
        setXservo();
        for (i=1;i<=180;i++){
            digitalWrite(mx_z, HIGH);
            delayMicroseconds(X_ang90);
            digitalWrite(mx_z, LOW);
            delayMicroseconds(20000 - X_ang90);
            delay(1);
        }
        if (!getTCRTbot()) break;
    }
    setXservo();
    for (i=1;i<=180;i++){
        // подадим на X servo импульс № i шириной X_angle90
        digitalWrite(mx_s1,HIGH);
        digitalWrite(mx_z, HIGH);
        delayMicroseconds(X_ang90);
        digitalWrite(mx_z, LOW);
        // подадим на Z servo импульс № i шириной Z_angle00
        digitalWrite(mx_s1,LOW);
        digitalWrite(mx_z, HIGH);
        delayMicroseconds(Z_ang00);
        digitalWrite(mx_z, LOW);
        delayMicroseconds(20000 - X_ang90 - Z_ang00);
        delay(1);
    }
    setXservo();
    moveservo(X_ang00);
    /* Timeout...??? - Oh f..ck */
    if (j == 100) return 0; // -2
    // wait, while next good down to Z level
    i = 0;
    do
    {
        i++;
        if (getTCRTbot()) break;    
    } while (i < waitout);
    if (i == waitout) return 0; // -3
    return 0;
}