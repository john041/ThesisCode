#include <ESP8266WiFi.h>                      //Import the wifi library
#include <PubSubClient.h>

const char* nameWifi = "Test";                //Create constant global variable for wifi name
const char* password = "TempPassword";        //Create constant global variable for wifi password

WiFiClient wifiClient;
PubSubClient MQTTClient(wifiClient);

const int trigPin = 12;                                               //Define what pins are being used
const int echoPin = 13;
float distance = 0;
float sensorTime = 0; 

String currentMessage = "";

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
  MQTTClient.publish("/test/reciever", "HTTP/1.1 200 OK  ");
  Serial.println(topic);
  currentMessage = "";
  for(int i = 0; i < length; i++) {
    currentMessage += (char)message[i];
  }
  Serial.println(currentMessage);
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
  digitalWrite(trigPin, 0);
}

void loop() {
    MQTTClient.loop();

    delay(1000);                                                      //Delay by one second
    getDistance();
    Serial.print(distance);                                           //Print out distance and inches to serial
    Serial.println(" inches");
}
