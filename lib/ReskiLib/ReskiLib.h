//  0 clip
//	{0,0,0,0} channel 0 - Z-servo 
//  {1,0,0,0} channel 1 - TCRT5000 bottom
//  {0,1,0,0} channel 2 - X-servo
//  {1,1,0,0} channel 3 - TCRT5000 top
//	1 clip
//  {0,0,1,0} channel 4 - Z-servo
//  {1,0,1,0} channel 5 - TCRT5000 bottom
//  {0,1,1,0} channel 6 - X-servo
//  {1,1,1,0} channel 7 - TCRT5000 top
// 	2 clip
//  {0,0,0,1} channel 8 - Z-servo
//  {1,0,0,1} channel 9 - TCRT5000 bottom
//  {0,1,0,1} channel 10 - X-servo
//  {1,1,0,1} channel 11 - TCRT5000 top
//	3 clip
//  {0,0,1,1} channel 12 - Z-servo
//  {1,0,1,1} channel 13 - TCRT5000 bottom
//  {0,1,1,1} channel 14 - X-servo
//  {1,1,1,1} channel 15 - TCRT5000 top
#pragma once
#include <Arduino.h>

#ifndef mx_en
    #define mx_en A2
#endif
#ifndef mx_z
    #define mx_z  5
#endif
#ifndef mx_s0
    #define mx_s0 9
#endif
#ifndef mx_s1
    #define mx_s1 8
#endif
#ifndef mx_s2
    #define mx_s2 7
#endif
#ifndef mx_s3
    #define mx_s3 6
#endif
// Angle of rotate X & Z Servo
#ifndef X_ang00
    // #define X_ang00  570 = MG995
    // SG90S
    #define X_ang00 2400        // 350 это реальный 0
#endif
#ifndef X_ang90
    // #define X_ang90 1250 - MG995
    // SG90S
    #define X_ang90 1520
#endif
#ifndef Z_ang00
    #define Z_ang00  544
#endif
#ifndef Z_ang90
    #define Z_ang90 1520
#endif

#ifndef waitout
    #define waitout 20000
#endif
// move attached servo to mx_z pin`s on "angle" without lib
void moveservo(int angle);
// one clip used 4 channel multiplex CD74HC4067. For work is once - must set pin S2 and S3. This function is made it.
void selclip(int clp);
// set S0 & S1 pins for select servo X, Z, TCRT5000 bottom & top
// X servo selected S0 = 0, S1 = 1
void setXservo();
// Z servo selected S0 = 0, S1 = 0
void setZservo();
// void forkdelay(int de_time);
// TCRT5000 on bottom selected & read value S0 = 1, S1 = 0
boolean getTCRTbot();
// TCRT5000 on top selected & read value S0 = 1, S1 = 1
boolean getTCRTtop();
// ReskiOut- out from selected clip 1 goods
int ReskiOut(int clp);
int ReskiOut2(int clp);