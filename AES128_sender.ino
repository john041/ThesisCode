#include <ESP8266WiFi.h>                      //Libraries Used
#include <PubSubClient.h>
#include <AES.h>
#include <Crypto.h>

const char* nameWifi = "Test";                //Name and password of network
const char* password = "TempPassword";

WiFiClient wifiClient;                        //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);          //Object to set up MQTT client
AES128 aes;                                   //Object of AES encryption

unsigned long roundTripTime = 0;              //Variables to keep track of time and number of packets sent
unsigned long startRoundTripTime = 0;
unsigned long encryptTime = 0;
unsigned long startEncryptTime = 0;
unsigned long decryptTime = 0;
unsigned long startDecryptTime = 0;
int number = 0;

char key[] = "Thisisatestaaaa!";               //Key used in encryption process

byte keyMemory[16];                           //Variables to store encryption information
byte messageMemory[16];
byte encryptedData[16];
byte decryptedData[16];

//Converts a string into a byte array
//  parameter 1 string to convert
//  parameter 2 byte array to copy to
void convertFromString( const char* info, byte* memory ) {
  for( int i = 0; i < strlen(info); i++ ) {
    memory[i] = (byte)info[i];
  }
}

void printByte( byte* info, int sizeOfArray) {
  for( int i = 0; i < sizeOfArray; i++ ) {
    Serial.write(info[i]);
  }
}

void runEncryption(const unsigned char* payload) {
   startEncryptTime = micros();
   aes.encryptBlock(encryptedData, payload);
   encryptTime = micros() - startEncryptTime;
}

void runDecryption(const unsigned char* payload) {
   startDecryptTime = micros();
   aes.decryptBlock(decryptedData, payload);
   decryptTime = micros() - startDecryptTime;
}

void recieve(char* topic, byte* message, unsigned int length) {
  //Serial.println(topic);
  roundTripTime = micros() - startRoundTripTime;
  Serial.print("Packet Sent:#");
  Serial.print(number);
  Serial.print("#Round Trip Time#");
  Serial.print(roundTripTime);
  Serial.print("#Encryption Time Sender#");
  Serial.print(encryptTime);
  Serial.print("#Encryption Time Sender#");
  Serial.print(decryptTime);
  Serial.print("#Message:#");
  runDecryption(message);
  printByte(message, length);
  printByte(decryptedData, sizeof(decryptedData));
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                                                 //Set clock for serial

  Serial.println("Connecting to WiFi");                                 
  WiFi.begin(nameWifi, password);                                     //Connect to Wifi Network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("-");
  }
  
  Serial.println("Wifi connected");                                   //Send message that Wifi connection   

  MQTTClient.setServer("192.168.1.113", 1883);
  MQTTClient.setCallback(recieve);
  
  if(MQTTClient.connect("Sensor")) {
    Serial.println("Connected to Broker");    
  } else {
    Serial.println("Problem connecting to Broker");
  }

  MQTTClient.subscribe("/test/reciever");
  convertFromString(key, keyMemory);
  aes.setKey(keyMemory, 16);
}

//main code here, runs repeatedly
void loop() {
  MQTTClient.loop();
  delay(4000);
  if(number < 200) {
    number = number + 1;
    convertFromString("SendDistAndTimes", messageMemory);
    startRoundTripTime = micros();
    runEncryption(messageMemory);
    bool result = MQTTClient.publish("/test/sender", encryptedData, sizeof(encryptedData), false);                 
  }
}
