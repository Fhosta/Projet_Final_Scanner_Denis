#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 16;        // Define pin D0 for the RST pin
constexpr uint8_t SDA_PIN = 15;        // Define pin D8 for the SDA pin

// Broches des LED
const int ledPin1 = 5; // Define pin D1 
const int ledPin2 = 4; // Define pin D2

byte readCard[4];
String MasterTag = "A5FB433";
String TagThomas = "75FD633";  // Tag ID of your RFID card (TO BE SUBSTITUTED)
String tagID = "";

MFRC522 mfrc522(SDA_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);	// Initialize serial communications with the PC
	while (!Serial);	// Do nothing if no serial port is opened
	SPI.begin();		// Initialize SPI bus
	mfrc522.PCD_Init();	// Initialize MFRC522

  // Initialisation des broches de sortie pour les LED
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

}

boolean getUID();

void loop() 
{
  //Wait until new tag is available
  while (getUID()) 
  {
     Serial.print(tagID);
    if (tagID == MasterTag) 
    { 
      Serial.println("Le grand maître");
      digitalWrite(ledPin1, HIGH);
      delay(1000);
      digitalWrite(ledPin1, LOW);
      delay(1000);
    }
    else if(tagID == TagThomas)
    {
      Serial.println("Bienvenue Thomas");
      digitalWrite(ledPin1, HIGH);
      delay(1000);
      digitalWrite(ledPin1, LOW);
      delay(1000);
    }

    else
    {
      Serial.println("Tu n'a pas les autorisations nécessaire !!!!!!!!!");
      digitalWrite(ledPin2, HIGH);
      delay(1000);
      digitalWrite(ledPin2, LOW);
      delay(1000);
    } 
    delay(2000);
  }
}
  
  	boolean getUID() 
  {
    // Getting ready for reading Tags
    if ( ! mfrc522.PICC_IsNewCardPresent()) {   //If a new tag is placed close to the RFID reader, continue
    return false;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {     //When a tag is placed, get UID and continue
    return false;
    }
    tagID = "";
    for ( uint8_t i = 0; i < 4; i++) {                  // The MIFARE tag in use has a 4 byte UID
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));  // Adds the 4 bytes in a single string variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
  }