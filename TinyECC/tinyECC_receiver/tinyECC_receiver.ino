#include <ESP8266WiFi.h>                  //Libraries Used
#include <PubSubClient.h>
#include <tinyECC.h>

const char* nameWifi = "Test";            //Name and password of network
const char* password = "TempPassword";    

WiFiClient wifiClient;                    //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);      //Object to set up MQTT client
tinyECC tinyEcc;                          //Object of AES encryption

const int trigPin = 12;                   //Define what pins are used to control distance sensor
const int echoPin = 13;
float distance = 0;                       //Variables to calulate and save the distance
float sensorTime = 0; 

unsigned long encryptTime = 0;
unsigned long startEncryptTime = 0;
unsigned long decryptTime = 0;
unsigned long startDecryptTime = 0;

char decryptedData[96];
char encryptedData[300];


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
void runEncryption(const char* payload) {
   startEncryptTime = micros();
   tinyEcc.plaintext = payload;
   tinyEcc.encrypt();                                            //Encrypt the char array
   encryptTime = micros() - startEncryptTime;                    //Messure the time to encrypt
   strcpy(encryptedData, (tinyEcc.ciphertext).c_str());
}

//Decrypts a char array while messuring time and free memory
//  parameter 1 const char array to decrypt
void runDecryption(const char* payload) {
   startDecryptTime = micros();
   tinyEcc.ciphertext = payload;
   tinyEcc.decrypt();                                            //Encrypt the char array
   decryptTime = micros() - startDecryptTime;                    //Messure the time to encrypt
   strcpy(decryptedData, (tinyEcc.plaintext).c_str());
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
  runDecryption((char*)message);                                           //Decrypt MQTT message
  printByte(message, length);
  Serial.println();
  Serial.println(decryptedData);                                    //Print decrypted message
  char tempDistance[5];
  char tempEncryption[5];
  byte messageToEncrypt[11];
  dtostrf(distance, 5, 2, tempDistance);                            //Puts distance and decryption time in byte array
  memcpy(messageToEncrypt, tempDistance, strlen(tempDistance));
  dtostrf(decryptTime, 4, 0, tempEncryption);
  messageToEncrypt[5] = '#';
  memcpy(messageToEncrypt + 6, tempEncryption, strlen(tempEncryption));
  runEncryption((char*)messageToEncrypt);                           //Encrypts new message
  printByte(messageToEncrypt, sizeof(messageToEncrypt));
  Serial.println();
  Serial.println(encryptedData);
  MQTTClient.publish("/test/reciever", encryptedData);              //Sends MQTT message
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

  MQTTClient.setBufferSize(1024);
  Serial.println(MQTTClient.getBufferSize());
  MQTTClient.subscribe("/test/sender");                   //Subscribe to topic to listen to
  digitalWrite(trigPin, 0);                               //Set output pin to zero
}

//Main code here, runs repeatedly
void loop() {
    MQTTClient.loop();                                    //Check for MQTT messages

    getDistance();                                        //Get the distance from sensor
    Serial.print(distance);                               //Print out distance and inches to serial
    Serial.println(" inches");
}
