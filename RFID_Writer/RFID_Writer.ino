/*
 * This tag allows us to write anything we want to a rfid tag. 
 * Tags can be read using the RFID_Reader.ino sketch
*/
#include <SPI.h>
#include <MFRC522.h>

//default pins
int rst_pin=9; 
int ss_pin =10; 
MFRC522 mfrc(ss_pin,rst_pin); //new instance of MFRC522

int led = LED_BUILTIN; //some pin we want to use
void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  Serial.begin(9600); 
  SPI.begin();
  mfrc.PCD_Init(); 
  mfrc.PCD_DumpVersionToSerial();
}

void loop() {

  //Initialize Stuff
  // put your main code here, to run repeatedly:
    // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  
 //______________________________________________________________________________________________________________
 //check for card/tag
  if ( ! mfrc.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc.PICC_ReadCardSerial()) {
    return;
  }

 //______________________________________________________________________________________________________________
 //Show some info about the tag
  Serial.print("Card UID:");    //Dump UID
  for (byte i = 0; i < mfrc.uid.size; i++) {
    Serial.print(mfrc.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc.uid.uidByte[i], HEX);
  }
  Serial.println();

 //______________________________________________________________________________________________________________
   byte buffer[34] = "Lukas"; //Change this to whatever we want
   byte  block; 
   MFRC522::StatusCode status;
  
 //______________________________________________________________________________________________________________
 //Authentification
    Serial.println("Authentification"); 
    block = 1; 
    status = mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc.uid)); 
    
    if (status != MFRC522::STATUS_OK) {
    Serial.print("PCD_Authenticate() failed: ");
    return;
    }  else Serial.println("PCD_Authenticate() success: ");

 //______________________________________________________________________________________________________________
 //write to tag
    status = mfrc.MIFARE_Write(block, buffer, 16);
     if (status != MFRC522::STATUS_OK) {
    Serial.print("MIFARE_Write() failed: ");
    return;
    }  else Serial.println("MIFARE_Write() success: ");
}
