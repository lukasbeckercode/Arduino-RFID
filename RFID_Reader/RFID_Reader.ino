/*
 * This sketch reads a RFID Tag and can be modified to act differently depending on whitch tag is presented to it. 
 * To write to a rfid tag use the RFID_Writer.ino file
 * By Lukas Becker, 2020
*/

#include <SPI.h>
#include <MFRC522.h>
//default pins
int rst_pin=9; 
int ss_pin =10; 

//new instance of MFRC522

MFRC522 mfrc(ss_pin,rst_pin); 

int led = LED_BUILTIN; //CHANGE TO WANTED PIN

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(9600); 
  SPI.begin();
  mfrc.PCD_Init(); 
  mfrc.PCD_DumpVersionToSerial();
}

int readRFID()
{
  //Initialize the RFID Board
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
 //______________________________________________________________________________________________________________
 //Check for cards 
    if ( ! mfrc.PICC_IsNewCardPresent()) {
    return 0;
  }

  // Select one of the cards
  if ( ! mfrc.PICC_ReadCardSerial()) {
    return 0;
  }
 //______________________________________________________________________________________________________________
 //Some variables we need
  byte buffer1[128];
  byte block = 4;
  byte len = 34;
  String msg;
  MFRC522::StatusCode status;
 //______________________________________________________________________________________________________________
 // Authenticate the RFID Tag
     
    status = mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc.uid)); 
    
    if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    return;
    }  else Serial.println("PCD_Authenticate() success: ");

 //______________________________________________________________________________________________________________
 //Read from Tag 
    status = mfrc.MIFARE_Read(block, buffer1, &len);
     if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Read() failed: ");
     Serial.println(mfrc.GetStatusCodeName(status));
    return;
    }  else Serial.println("MIFARE_Read() success: ");

 //______________________________________________________________________________________________________________    
  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]); //print what we read
      msg.concat((char) buffer1[i]); //write the characters to a String
    }
  }
  Serial.print(" ");

   Serial.println("\n**End Reading**\n");

 //______________________________________________________________________________________________________________
  
  delay(1000); //change value if you want to read cards faster

  mfrc.PICC_HaltA(); //Reset the reader
  mfrc.PCD_StopCrypto1();
  
  msg.trim(); //remove whitespaces from String
  //Serial.println(msg); Debugging
     if(msg == "Lukas") //change to whatever you want
  {
  return 1;
  } else 
  {return 0;
  }
}


 //______________________________________________________________________________________________________________
void loop() {
  // put your main code here, to run repeatedly:
if(readRFID() == 1)
{
  digitalWrite(led,1); //Do something
}


}
