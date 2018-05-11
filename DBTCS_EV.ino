#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);

int red1 = 2;
int green1 = 3;
int red2 =  7;
int green2 = 6;
int sen1 = 0;
int sen2 = 0;
int sen3 = 0;
int sen4 = 0;
void setup() {

  Serial.begin(9600); 
  SPI.begin();
  mfrc522.PCD_Init();
  
  pinMode(red1, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(red2,OUTPUT);
  pinMode(green2,OUTPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);

  analogWrite(red1, 0);
  analogWrite(green1, 255);
  analogWrite(red2,255);
  analogWrite(green2,0);
  delay(3000);
}
void turn_green1(int d) {   //to turn on green for Lane 1
  analogWrite(red1, 255);
  analogWrite(green1, 0);
  delay(d);
  analogWrite(red1, 0);
  analogWrite(green1, 255);
}
void turn_green2(int d){    //to turn on green for Lane 2
  analogWrite(red2, 255);
  analogWrite(green2, 0);
  delay(d);
  analogWrite(red2, 0);
  analogWrite(green2, 255);
}
void turn_green2s(int d){
  analogWrite(red2, 255);
  analogWrite(green2, 0);
  delay(d);
}

void signal1(){                     //check density and turn green accordingly for Lane 1
  if (sen1 < 400 && sen2 < 400) {
    Serial.print("Sen1:");
    Serial.println(sen1);
    Serial.print("Sen2:");
    Serial.println(sen2);
    turn_green1(9000);            //delay changes depending on the number of vehicles(Density) on a particular lane
  }
  else if (sen1 < 400 || sen2 < 400) {
    Serial.print("Sen1:");
    Serial.println(sen1);
    Serial.print("Sen2:");
    Serial.println(sen2);
    turn_green1(5000);
  }
  else {
    Serial.print("Sen1:");
    Serial.println(sen1);
    Serial.print("Sen2:");
    Serial.println(sen2);
    turn_green1(2000);
  }
}
void signal2(){                   //check density and turn green accordingly for Lane 2
  if (sen3 < 400 && sen4 < 400) {
    Serial.print("Sen3:");
    Serial.println(sen3);
    Serial.print("Sen4:");
    Serial.println(sen4);
    turn_green2s(2000);
    rfidd();
    turn_green2(6000);
  }
  else if (sen3 < 400 || sen4 < 400) {
    Serial.print("Sen3:");
    Serial.println(sen3);
    Serial.print("Sen4:");
    Serial.println(sen4);
    turn_green2(5000);
  }
  else {
    Serial.print("Sen3:");
    Serial.println(sen3);
    Serial.print("Sen4:");
    Serial.println(sen4);
    turn_green2(2000);
  }
}
void rfidd(){                               //Check for Emergency Vehicles at Lane 1 when Lane 2 has more Density 
   Serial.println("checking: ");
   delay(2000);
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    Serial.print("No Emergency");
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
    String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "E3 9F 25 D9")    //Unique Identification tag attached to Emergency Vehicles
  {
    Serial.println("Emergency");
     analogWrite(red1,255);
    analogWrite(green1,0);
    analogWrite(red2,0);
    analogWrite(green2,255);
    delay(10000);
    analogWrite(red1,0);
    analogWrite(green1,255);
  }
 
 else   {
    Serial.println(" Access denied");   //has a tag but not registered as Emergency Vehicle
    delay(3000);
      }
   }

void loop() {

  
  sen1 = analogRead(A3);
  sen2 = analogRead(A4);    //finding number of vehicles at Lane 1
  
  analogWrite(red2, 0);
  analogWrite(green2, 255);
  signal1();
  
  sen3=analogRead(A0);    //finding number of vehilces at Lane 2
  sen4=analogRead(A1);

  analogWrite(red1, 0);
  analogWrite(green1, 255);
  signal2();
  
}


