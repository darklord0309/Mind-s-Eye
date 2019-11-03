#include <Servo.h>

#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t records[7]; // save record
uint8_t buf[64];
Servo s1;
Servo s2;
Servo s3;
Servo s4;

Servo s5;
int pos = 0;  
int leye = 13;
int reye =12;

#define onRecord    (1)
#define offRecord   (2)
#define eyemaar     (3)
#define walk        (4)
#define Stop        (5)
/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** initialize */
  myVR.begin(9600);
  s1.attach(9);//hand
s2.attach(11);//hand

s5.attach(9);//neck
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(leye, OUTPUT);
  pinMode(reye, OUTPUT);  
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)onRecord) >= 0){
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)offRecord) >= 0){
    Serial.println("offRecord loaded");
  }
  if(myVR.load((uint8_t)eyemaar) >= 0){
    Serial.println("eyemaar loaded");
  }
  if(myVR.load((uint8_t)walk) >= 0){
    Serial.println("walk loaded");
  }
  if(myVR.load((uint8_t)Stop) >= 0){
    Serial.println("stop loaded");
  }
}

void loop()
{
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case onRecord:
      Serial.println("USER VOICE FOUND");
        /** baba wake up */
        digitalWrite(leye, HIGH);
        digitalWrite(reye, HIGH);
         for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    s1.write(pos);
        s2.write(pos); 
    // tell servo to go to position in variable 'pos'
    delay(15);  
    
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    s1.write(pos); 
        s2.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);            // waits 15ms for the servo to reach the position
  }
  
        break;
        
      case offRecord:
      Serial.println("HIGH PEST PROBABILITY");
        /** baba good night*/
        
        digitalWrite(leye, LOW);
        digitalWrite(reye, LOW);
        break;
        case eyemaar:
        /** ae aakh maar k dikha */
        Serial.println("HIGH PEST PROBABILITY");
        digitalWrite(leye, LOW);
        digitalWrite(reye, HIGH);
        delay (175);
        digitalWrite(leye,HIGH);
        break;
        case walk:
        {//walk baba
          Serial.println("HIGH PEST PROBABILITY");
          digitalWrite(leye, HIGH);
        digitalWrite(reye, HIGH);
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    s1.write(pos);
        s2.write(pos); 
    // tell servo to go to position in variable 'pos'
    delay(15);  
    
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    s1.write(pos); 
        s2.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);            // waits 15ms for the servo to reach the position
  }
  
 }
case Stop:
{//stop
 
  Serial.println("HIGH PEST PROBABILITY");
  s1. write(0);
  
  
  s3.detach();
  
  
  s4.detach();
  
  
  s2.write(0);
  s5.write(0);

  
  
  
   }
 default:
        Serial.println("ALL COOL HERE,I CAN HEAR THE BIRDS CHIRPING");
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}
