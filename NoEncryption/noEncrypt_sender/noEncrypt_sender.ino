#include <ESP8266WiFi.h>               //Libraries Used                                    //Included Libraries
#include <PubSubClient.h>

const char* nameWifi = "Test";         //Name and password of network
const char* password = "TempPassword";

WiFiClient wifiClient;                 //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);   //Object to set up MQTT client

String currentMessage = "";
unsigned long roundTripTime = 0;
unsigned long startRoundTripTime = 0;
//unsigned long encryptTime = 0;
//unsigned long startEncryptTime = 0;
//unsigned long decryptTime = 0;
//unsigned long startDecryptTime = 0;
int number = 0;

void recieve(char* topic, byte* message, unsigned int length) {
  //Serial.println(topic);
  roundTripTime = micros() - startRoundTripTime;
  Serial.print("Packet Sent:#");
  Serial.print(number);
  Serial.print("#Round Trip Time#");
  Serial.print(roundTripTime);
  //Serial.print("#Encryption Time Sender#");
  //Serial.print(encryptTime);
  //Serial.print("#Encryption Time Sender#");
  //Serial.print(decryptTime);
  Serial.print("#Message:#");
  currentMessage = "";
  for(int i = 0; i < length; i++) {
    currentMessage += (char)message[i];
  }
  Serial.println(currentMessage);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  MQTTClient.loop();
  delay(4000);
  if(number < 200) {
    number = number + 1;
    startRoundTripTime = micros();
    bool result = MQTTClient.publish("/test/sender", "GET /index.php?stop=Stop HTTP/1.1");                 
  }
}
