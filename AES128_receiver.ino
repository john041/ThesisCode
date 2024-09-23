#include <ESP8266WiFi.h>                                //Import the wifi library
#include <PubSubClient.h>
#include <AES.h>
#include <Crypto.h>

const char* nameWifi = "Test";                //Create constant global variable for wifi name
const char* password = "TempPassword";                  //Create constant global variable for wifi password

WiFiClient wifiClient;
PubSubClient MQTTClient(wifiClient);
AES128 aes;

const int trigPin = 12;                                               //Define what pins are being used
const int echoPin = 13;
float distance = 0;
float sensorTime = 0; 

String currentMessage = "";
unsigned long encryptTime = 0;
unsigned long startEncryptTime = 0;
unsigned long decryptTime = 0;
unsigned long startDecryptTime = 0;

char key[] = "Thisisatestaaaa!";               //Key used in encryption process

byte keyMemory[16];                           //Variables to store encryption information
byte messageMemory[16];
byte encryptedData[16];
byte decryptedData[16];

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

void getDistance() {
  int average[10];                                               //Create an array to store time to find average
  for(int i = 0; i < 10; i++) {                                  //Loop to get multiple times
    digitalWrite(trigPin, 1);                                         //Turn on trigger pin
    delay(15);                                                        //Delay by 14 miliseconds
    digitalWrite(trigPin, 0);                                         //Turn off trigger pin
    average[i] = pulseIn(echoPin, 1);                                 //Set the input time to the average array
  }
  int sumTime = 0;
  for(int i = 0; i < 10; i++) {                                  //Create a sum from the time average array
    sumTime = sumTime + average[i];
  }
  sensorTime = sumTime / 10;                                     //FInd the average time for last time 
  distance = sensorTime * 0.0135 / 2; 
}

void recieve(char* topic, byte* message, unsigned int length) {
  runDecryption(message);
  printByte(message, length);
  Serial.println();
  printByte(decryptedData, sizeof(decryptedData));
  Serial.println();
  char tempDistance[5];
  char tempEncryption[5];
  byte messageToEncrypt[10];
  dtostrf(distance, 4, 3, tempDistance);
  dtostrf(decryptTime, 4, 0, tempEncryption);
  memcpy(messageToEncrypt, tempDistance, sizeof(tempDistance));
  memcpy(messageToEncrypt + sizeof(tempDistance), tempEncryption, sizeof(tempEncryption));
  runEncryption(messageToEncrypt);
  printByte(messageToEncrypt, sizeof(messageToEncrypt));
  Serial.println();
  printByte(encryptedData, sizeof(encryptedData));
  MQTTClient.publish("/test/reciever", encryptedData, sizeof(encryptedData), false);
  Serial.println(topic);
  currentMessage = "";
  for(int i = 0; i < length; i++) {
    currentMessage += (char)message[i];
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT);                                           //Set pins for output and input
  pinMode(echoPin, INPUT);

  Serial.println("Connecting to WiFi");                                 
  WiFi.begin(nameWifi, password);                                       //Connect to wifi network
  while (WiFi.status() != WL_CONNECTED) {                               //Loop untill the wifi network is connected
    Serial.print("-");
  }

  Serial.println("Wifi connected");

  MQTTClient.setServer("192.168.1.113", 1883);
  MQTTClient.setCallback(recieve);
  
  if(MQTTClient.connect("Reciever")) {
    Serial.println("Connected to Broker");    
  } else {
    Serial.println("Problem connecting to Broker");
  }

  MQTTClient.subscribe("/test/sender");
  convertFromString(key, keyMemory);
  aes.setKey(keyMemory, 16);
  digitalWrite(trigPin, 0);
}

void loop() {
    MQTTClient.loop();

    delay(1000);                                                      //Delay by one second
    getDistance();
    Serial.print(distance);                                           //Print out distance and inches to serial
    Serial.println(" inches");
}
