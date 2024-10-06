#include <ESP8266WiFi.h>                  //Libraries Used
#include <PubSubClient.h>
#include <present.h>

const char* nameWifi = "Test";            //Name and password of network
const char* password = "TempPassword";    

WiFiClient wifiClient;                    //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);      //Object to set up MQTT client

const int trigPin = 12;                   //Define what pins are used to control distance sensor
const int echoPin = 13;
float distance = 0;                       //Variables to calulate and save the distance
float sensorTime = 0; 

unsigned long encryptTime = 0;            //Variables to keep track of time and free memory
unsigned long startEncryptTime = 0;
unsigned long decryptTime = 0;
unsigned long startDecryptTime = 0;

char key[] = "Thisisatestaaaa!";          //Key used in encryption process

byte keyMemory[16];                       //Variables to store encryption information
byte decryptedMemory[16];
byte encryptedMemory[16];

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
void runEncryption() {
   int length = sizeof(encryptedMemory);
   startEncryptTime = micros();
   for( int i = 0; i < length; i += 8) {
      present_encrypt(encryptedMemory + i, keyMemory);             //Encrypt the char array
   }
   encryptTime = micros() - startEncryptTime;                     //Messure the time to encrypt
}

//Decrypts a char array while messuring time and free memory
void runDecryption() {
   int length = sizeof(decryptedMemory);
   startDecryptTime = micros();
   for( int i = 0; i < length; i += 8) {
     present_decrypt(decryptedMemory + i, keyMemory);              //Encrypt the char array
   }
   decryptTime = micros() - startDecryptTime;                     //Messure the time to encrypt
}

//Calculates distance from the distance sensor 
void getDistance() {
  int average[10];                                              //Create an array to store time to find average
  for(int i = 0; i < 10; i++) {                                 //Loop to get multiple times
    digitalWrite(trigPin, 1);                                   //Turn on trigger pin
    delay(15);                                                  //Delay by 14 miliseconds
    digitalWrite(trigPin, 0);                                   //Turn off trigger pin
    average[i] = pulseIn(echoPin, 1);                           //Set the input time to the average array
  }
  int sumTime = 0;
  for(int i = 0; i < 10; i++) {                                 //Create a sum from the time average array
    sumTime = sumTime + average[i];
  }
  sensorTime = sumTime / 10;                                    //Find the average time for last time 
  distance = sensorTime * 0.0135 / 2; 
}

//MQTT functions that runs when MQTT message is recieved
//  parameter 1 char pointer that stores the topic that the message comes from
//  parameter 2 byte pointer that contains the message of MQTT messsage
//  parameter 3 unsigned int of the length of the message
void recieve(char* topic, byte* message, unsigned int length) {
  memcpy(decryptedMemory, message, length);
  runDecryption();                                           //Decrypt MQTT message
  printByte(message, length);
  Serial.println();
  printByte(decryptedMemory, sizeof(decryptedMemory));                  //Print decrypted message
  Serial.println();
  char tempDistance[7];
  char tempEncryption[8];
  byte messageToEncrypt[16];
  dtostrf(distance, 7, 2, tempDistance);                            //Puts distance and decryption time in byte array
  memcpy(messageToEncrypt, tempDistance, strlen(tempDistance));
  dtostrf(decryptTime, 8, 0, tempEncryption);
  messageToEncrypt[7] = '#';
  memcpy(messageToEncrypt + 8, tempEncryption, strlen(tempEncryption));
  memcpy(encryptedMemory, messageToEncrypt, sizeof(messageToEncrypt));
  runEncryption();                                                           //Encrypts new message
  printByte(messageToEncrypt, sizeof(messageToEncrypt));
  Serial.println();
  printByte(encryptedMemory, sizeof(encryptedMemory));
  Serial.println();
  MQTTClient.publish("/test/reciever", encryptedMemory, sizeof(encryptedMemory), false);         //Sends MQTT message
}

//Setup code here, to run once:
void setup() {
  Serial.begin(9600);                                     //Set clock for serial

  pinMode(trigPin, OUTPUT);                               //Set pins for output and input
  pinMode(echoPin, INPUT);

  Serial.println("Connecting to WiFi");                                 
  WiFi.begin(nameWifi, password);                         //Connect to wifi network
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("-");
  }
  Serial.println("Wifi connected");                       //Send message that Wifi connection

  MQTTClient.setServer("192.168.1.113", 1883);            //MQTT settings setup
  MQTTClient.setCallback(recieve);
  
  if(MQTTClient.connect("Reciever")) {                    //Connect to MQTT broker
    Serial.println("Connected to Broker");    
  } else {
    Serial.println("Problem connecting to Broker");
  }

  MQTTClient.subscribe("/test/sender");                   //Subscribe to topic to listen to
  convertFromString(key, keyMemory);                      //Set up encryption key
  digitalWrite(trigPin, 0);                               //Set output pin to zero
}

//Main code here, runs repeatedly
void loop() {
    MQTTClient.loop();                                    //Check for MQTT messages

    getDistance();                                        //Get the distance from sensor
    Serial.print(distance);                               //Print out distance and inches to serial
    Serial.println(" inches");
}
