#include <ESP8266WiFi.h>                      //Libraries Used
#include <PubSubClient.h>
#include <Speck.h>
#include <Crypto.h>

const char* nameWifi = "Test";                //Name and password of network
const char* password = "TempPassword";

WiFiClient wifiClient;                        //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);          //Object to set up MQTT client
Speck speck;                                  //Object of AES encryption

unsigned long roundTripTime = 0;              //Variables to keep track of time, number of packets sent, and free memory
unsigned long startRoundTripTime = 0;
unsigned long encryptTime = 0;
unsigned long startEncryptTime = 0;
unsigned long decryptTime = 0;
unsigned long startDecryptTime = 0;
unsigned long freeMemoryEncrypt = 0;
unsigned long freeMemoryDecrypt = 0;
int number = 0;
int nextNum = 1;

char key[] = "Thisisatestaaaa!";               //Key used in encryption process

byte keyMemory[16];                           //Variables to store encryption information
byte messageMemory[96];
byte encryptedData[96];
byte decryptedData[16];

//Converts a string into a byte array
//  parameter 1 string to convert
//  parameter 2 byte array to copy to
void convertFromString( const char* info, byte* memory ) {
  for( int i = 0; i < strlen(info); i++ ) {
    memory[i] = (byte)info[i];
  }
}

//Prints a byte array to serial output
//  parameter 1 byte to print
//  parameter 2 int of the size of the byte array
void printByte( byte* info, int sizeOfArray) {
  for( int i = 0; i < sizeOfArray; i++ ) {
    Serial.write(info[i]);
  }
}

//Encrypts a char array while messuring time and free memory
//  parameter 1 const char array to encrypt
//  parameter 2 int length of payload
void runEncryption(const unsigned char* payload, int length) {
   startEncryptTime = micros();
   for(int i = 0; i < length; i += 16) {
     speck.encryptBlock(encryptedData + i, payload + i);         //Encrypt the char array
   }
   encryptTime = micros() - startEncryptTime;                    //Messure the time to encrypt
   freeMemoryEncrypt = ESP.getFreeHeap();                        //Messure the free memory space
}

//Decrypts a char array while messuring time and free memory
//  parameter 1 const char array to decrypt
//  parameter 2 int length of payload
void runDecryption(const unsigned char* payload, int length) {
   startDecryptTime = micros();
   for(int i = 0; i < length; i += 16) {
     speck.decryptBlock(decryptedData + i, payload + i);         //Encrypt the char array
   }
   decryptTime = micros() - startDecryptTime;                    //Messure the time to encrypt
   freeMemoryDecrypt = ESP.getFreeHeap();                        //Messure the free memory space
}

//MQTT functions that runs when MQTT message is recieved
//  parameter 1 char pointer that stores the topic that the message comes from
//  parameter 2 byte pointer that contains the message of MQTT messsage
//  parameter 3 unsigned int of the length of the message
void recieve(char* topic, byte* message, unsigned int length) {
  runDecryption(message, length);                                       //Decrypt MQTT message
  roundTripTime = micros() - startRoundTripTime;
  number = number + 1;
  Serial.print("Packet Sent:#");                                //Print roundtrip time, encrypt time, and decrypt time
  Serial.print(number);
  Serial.print("#Round Trip Time#");
  Serial.print(roundTripTime);
  Serial.print("#Encryption Time Sender#");
  Serial.print(encryptTime);
  Serial.print("#Encryption Time Sender#");
  Serial.print(decryptTime);
  Serial.print("#Message:#");
  //printByte(message, length);
  Serial.print("#");
  printByte(decryptedData, strlen((char*)decryptedData));              //Print decrypted message and memory messurments
  Serial.print("#");
  Serial.print(freeMemoryEncrypt);
  Serial.print("#");
  Serial.print(freeMemoryDecrypt);
  Serial.print("#");
  Serial.println(ESP.getFreeHeap());
}

//Setup code here, to run once:
void setup() {
  Serial.begin(9600);                                            //Set clock for serial

  Serial.println("Connecting to WiFi");                                 
  WiFi.begin(nameWifi, password);                                //Connect to Wifi Network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("-");
  }
  Serial.println("Wifi connected");                              //Send message that Wifi connection   

  MQTTClient.setServer("192.168.1.113", 1883);                   //MQTT settings setup               
  MQTTClient.setCallback(recieve);
  
  if(MQTTClient.connect("Sensor")) {                             //Connect to MQTT broker
    Serial.println("Connected to Broker");    
  } else {
    Serial.println("Problem connecting to Broker");
  }

  MQTTClient.subscribe("/test/reciever");                       //Subscribe to topic to listen to
  convertFromString(key, keyMemory);                            //Set up encryption key
  speck.setKey(keyMemory, sizeof(keyMemory));
  delay(5000);
}

//Main code here, runs repeatedly:
void loop() {
  MQTTClient.loop();                                           //Check for MQTT messages
  if(number < 200) {                                           //Send 200 packets every 1 second
    if(number == (nextNum - 1)) {
      nextNum = nextNum + 1;
      convertFromString("This is a long sentence that is encrypted and then transmitted using the MQTT protocol for test.", messageMemory);
      startRoundTripTime = micros();
      runEncryption(messageMemory, sizeof(messageMemory));                              //Encrypt message and send message
      bool result = MQTTClient.publish("/test/sender", encryptedData, sizeof(encryptedData), false);                 
    }
  }
}
