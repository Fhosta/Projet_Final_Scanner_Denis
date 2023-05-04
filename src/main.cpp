///////////////////////////////////////////////Présentation /////////////////////////////////////

//Auteur : Florian Hostachy
//Date : 5/4/2023
//Fonction: identifie une personne est envoi sont nom et numéro de badge a notre api allume une lumière rouge si l'utilisateur est non connu sinon verte pour les autorisées

///////////////////////////////////////////////Présentation /////////////////////////////////////


// Fichier nécessaire
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>

WiFiClient client;

// Informations de connexion Wi-Fi
const char* ssid = "EcoleDuWeb2.4g";
const char* password = "EcoleDuWEB";

// Information pour l'api
const char* apiHost = "api.qc-ca.ovh";
const int apiPort = 2222;
const char* apiEndpoint = "/api/ajouter/jeton";

constexpr uint8_t RST_PIN = 16;   // Define pin D0 for the RST pin
constexpr uint8_t SDA_PIN = 15;   // Define pin D8 for the SDA pin

// Broches des LED
const int ledPin1 = 5; // Define pin D1 
const int ledPin2 = 4; // Define pin D2

byte readCard[4];
String MasterTag = "A5FB433";
String TagThomas = "75FD633";  // Tag ID of your RFID card (TO BE SUBSTITUTED)
String numtag = "";
String nom = "";
String requestBody = "{\"numtag\":\"";

MFRC522 mfrc522(SDA_PIN, RST_PIN);  // Create MFRC522 instance

boolean getUID();

void connectAndSendRequest(const char* apiHost, uint16_t apiPort, const char* apiEndpoint, const String& requestBody, const String& numtag,const String& nom) {
  if (client.connect(apiHost, apiPort)) {
    Serial.println("Connected to API");
    
    // Construction du corps de la requête JSON en incluant la valeur de numtag
    String fullRequestBody = "{\"numtag\": \"" + numtag + "\", \"nom\": \"" + nom + "\"}";


    // Envoi de la requête POST
    client.print(String("POST ") + apiEndpoint + " HTTP/1.1\r\n" +
                 "Host: " + apiHost + "\r\n" +
                 "Connection: close\r\n" +
                 "Content-Type: application/json\r\n" +
                 "Content-Length: " + String(fullRequestBody.length()) + "\r\n" +
                 "\r\n" +
                 fullRequestBody);

  
    // Lecture de la réponse de l'API
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
        // Faites ici ce que vous voulez avec les données de la réponse de l'API
      }
    }

    // Fermeture de la connexion
    client.stop();
    Serial.println("API disconnected");
  }
}

void setup() {
  Serial.begin(9600);  // Initialize serial communications with the PC
  while (!Serial);  // Do nothing if no serial port is opened
  SPI.begin();    // Initialize SPI bus
  mfrc522.PCD_Init();  // Initialize MFRC522

  // Initialisation des broches de sortie pour les LED
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

   // Connexion au réseau Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  Serial.println("Connecting to API...");
}

void loop() 
{
  //Wait until new tag is available
  while (getUID()) 
  {
     Serial.print(numtag);
    if (numtag == MasterTag) 
    { 
      nom = "Henri";
      digitalWrite(ledPin1, HIGH);
      delay(1000);
      digitalWrite(ledPin1, LOW);
      delay(1000);
      connectAndSendRequest(apiHost, apiPort, apiEndpoint, requestBody,numtag,nom);
    }
    else if(numtag == TagThomas)
    {
      nom = "Tony";
      digitalWrite(ledPin1, HIGH);
      delay(1000);
      digitalWrite(ledPin1, LOW);
      delay(1000);
      connectAndSendRequest(apiHost, apiPort, apiEndpoint, requestBody,numtag,nom);
    }

    else
    {
      digitalWrite(ledPin2, HIGH);
      delay(1000);
      digitalWrite(ledPin2, LOW);
      delay(1000);
    } 
    delay(2000);
  }
}


  boolean getUID() {
  // Getting ready for reading Tags
  if (!mfrc522.PICC_IsNewCardPresent()) { //If a new tag is placed close to the RFID reader, continue
  return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) { //When a tag is placed, get UID and continue
  return false;
  }
  numtag = "";
  for (uint8_t i = 0; i < 4; i++) { // The MIFARE tag in use has a 4 byte UID
  numtag.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single string variable
  }
  numtag.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
  }