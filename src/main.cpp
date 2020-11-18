#include "ReskiLib.h"
#include <avr/eeprom.h>
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
// 	volatile int stock[num_clip]={depth_clip, depth_clip, depth_clip, depth_clip};

#define num_clip 4
#define depth_clip 11

volatile int8_t stock[num_clip]={depth_clip, depth_clip, depth_clip, depth_clip};
volatile int8_t RM_state;
/*
void stock_print(){
      Serial.print(" stock[0]=");Serial.print(stock[0]);
      Serial.print(" stock[1]=");Serial.print(stock[1]);
      Serial.print(" stock[2]=");Serial.print(stock[2]);
      Serial.print(" stock[3]=");Serial.println(stock[3]);
}
*/
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Serial.println("Start!");
  // Enable multiplexor 
  pinMode(mx_en, OUTPUT);
	digitalWrite(mx_en, LOW);
  //Reading from EEPROM final state of ReskoMat
  //__EEGET(RM_state,0);
  // reading value of good in clip if clip not full
  for (int i=0;i<4; i++) {
    __EEGET(stock[i],i);
    selclip(i);
    if (stock[i]>depth_clip || !getTCRTtop()) {
      stock[i] = depth_clip;
      __EEPUT(i, stock[i]);
    }
  }
  //stock_print();
}

boolean A0_en, A1_en = true;

void loop() {
  int i, rc = 0; 
  // put your main code here, to run repeatedly:
  if (digitalRead(A0)) {
    if (A0_en) 
    {
      A0_en = false;
      //Serial.println("A0 Pressed");
      // find max filling clip 
      i = 0;
      for (int j = 1; j < 4; j++) if (stock[j] > stock[i]) i = j;
      if (stock[i] <= 0) {
        //Serial.print("All clip is empty or out of service");  
      } else {
        rc = ReskiOut(i);
        if (rc == 0) stock[i]--; else stock[i]=rc;
        __EEPUT(i, stock[i]);
        //Serial.print("ReskiOut RC = ");Serial.println(rc);
      }
      //stock_print();
    } 
  } else { A0_en = true; }

  if (digitalRead(A1)) {
    if (A1_en) 
    {
      A1_en = false;
      //Serial.println("A1 Pressed");
      // find max filling clip 
      i = 0;
      for (int j = 1; j < 4; j++) if (stock[j] > stock[i]) i = j;
      if (stock[i] <= 0) {
        //Serial.print("All clip is empty or out of service");  
      } else {
        rc = ReskiOut2(i);
        if (rc == 0) stock[i]--; else stock[i]=rc;
        __EEPUT(i, stock[i]);
        //Serial.print("ReskiOut RC = ");Serial.println(rc);
      }
      //stock_print();
    } 
  } else { A1_en = true; }
}