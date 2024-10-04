#include <ESP8266WiFi.h>                      //Libraries Used
#include <PubSubClient.h>
#include <tinyECC.h>

const char* nameWifi = "Test";                //Name and password of network
const char* password = "TempPassword";

WiFiClient wifiClient;                        //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);          //Object to set up MQTT client
tinyECC tinyEcc;                              //Object of AES encryption

unsigned long roundTripTime = 0;              //Variables to keep track of time, number of packets sent, and free memory
unsigned long startRoundTripTime = 0;
unsigned long encryptTime = 0;
unsigned long startEncryptTime = 0;
unsigned long decryptTime = 0;
unsigned long startDecryptTime = 0;
unsigned long freeMemoryEncrypt = 0;
unsigned long freeMemoryDecrypt = 0;
int number = 0;

char encryptedData[300];
char decryptedData[16];

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
   freeMemoryEncrypt = ESP.getFreeHeap();                        //Messure the free memory space
}

//Decrypts a char array while messuring time and free memory
//  parameter 1 const char array to decrypt
void runDecryption(const char* payload) {
   startDecryptTime = micros();
   tinyEcc.ciphertext = payload;
   tinyEcc.decrypt();                                            //Encrypt the char array
   decryptTime = micros() - startDecryptTime;                    //Messure the time to encrypt
   strcpy(decryptedData, (tinyEcc.plaintext).c_str());
   freeMemoryDecrypt = ESP.getFreeHeap();                        //Messure the free memory space
}

//MQTT functions that runs when MQTT message is recieved
//  parameter 1 char pointer that stores the topic that the message comes from
//  parameter 2 byte pointer that contains the message of MQTT messsage
//  parameter 3 unsigned int of the length of the message
void recieve(char* topic, byte* message, unsigned int length) {
  runDecryption((char*)message);                                       //Decrypt MQTT message
  roundTripTime = micros() - startRoundTripTime;
  Serial.print("Packet Sent:#");                                //Print roundtrip time, encrypt time, and decrypt time
  Serial.print(number);
  Serial.print("#Round Trip Time#");
  Serial.print(roundTripTime);
  Serial.print("#Encryption Time Sender#");
  Serial.print(encryptTime);
  Serial.print("#Decryption Time Sender#");
  Serial.print(decryptTime);
  Serial.print("#Message:#");
  printByte(message, length);
  Serial.print("#");
  Serial.print(decryptedData);                               //Print decrypted message and memory messurments
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
  MQTTClient.setBufferSize(2024);
  Serial.println(MQTTClient.getBufferSize());
  MQTTClient.subscribe("/test/reciever");                       //Subscribe to topic to listen to
}

//Main code here, runs repeatedly:
void loop() {
  MQTTClient.loop();                                           //Check for MQTT messages
  delay(1000);
  if(number < 200) {                                           //Send 200 packets every 1 second
    number = number + 1;
    startRoundTripTime = micros();
    runEncryption("SendDistAndTimes");                              //Encrypt message and send message
    MQTTClient.publish("/test/sender", encryptedData);                 
  }
}
