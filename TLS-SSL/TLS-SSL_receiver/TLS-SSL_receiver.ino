#include <ESP8266WiFi.h>                  //Libraries Used
#include <PubSubClient.h>
#include <time.h>

const char* nameWifi = "Test";            //Name and password of network
const char* password = "TempPassword";    

const char* ntp_server = "pool.ntp.org";
const long gmt_offset_sec = -25200;
const int daylight_offset_sec = 0;

BearSSL::WiFiClientSecure wifiClient;                    //Object to set up Wifi
PubSubClient MQTTClient(wifiClient);      //Object to set up MQTT client

const int trigPin = 12;                   //Define what pins are used to control distance sensor
const int echoPin = 13;
float distance = 0;                       //Variables to calulate and save the distance
float sensorTime = 0; 

byte messageMemory[16];

static const char ca_cert[]
PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIUFsAEHSPkH54OwVIYmzjqr6+s7IYwDQYJKoZIhvcNAQEL
BQAwZzELMAkGA1UEBhMCVVMxDTALBgNVBAgMBFV0YWgxDjAMBgNVBAcMBVByb3Zv
MSEwHwYDVQQKDBhJbnRlcm5ldCBXaWRnaXRzIFB0eSBMdGQxFjAUBgNVBAMMDTE5
Mi4xNjguMS4xMTMwHhcNMjQxMTIxMDY0MjUzWhcNMjUxMTIxMDY0MjUzWjBnMQsw
CQYDVQQGEwJVUzENMAsGA1UECAwEVXRhaDEOMAwGA1UEBwwFUHJvdm8xITAfBgNV
BAoMGEludGVybmV0IFdpZGdpdHMgUHR5IEx0ZDEWMBQGA1UEAwwNMTkyLjE2OC4x
LjExMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAL8pJkBIWURHXVyz
ID/zS0Jcx2k3VvEW+Dip2I/7sKd+/Jx81LQwzeVotMUtNspPmOBJjebAJOoKDvBh
AyxqzLkVv+XrbpjefA/HGXm5rMtiaTQ7ACnfRrMVOIAxvB5E8E6vBYWqPOf/bzFW
vCsXq+/Z6kuZ5/aMn92LvSo1V8GiU+4Ryj9x8NgaIKkOwkt7ulwkyoW9QJNxdohz
URl+1KL5wrSx+rtUHStn/Yfe34V+4mLV9Rh2BcH0ySrD2Lg1xQ5TnOPYQIa4rT8K
31zSCdopa6UaE+4LzYUOhD2QPLaxdSr578aqKghAxYl2S0uDDtUMqj+3jnDhaP8g
xLSj+0UCAwEAAaNTMFEwHQYDVR0OBBYEFN2+iqYE1ST+wU60zwmitd3/3SwEMB8G
A1UdIwQYMBaAFN2+iqYE1ST+wU60zwmitd3/3SwEMA8GA1UdEwEB/wQFMAMBAf8w
DQYJKoZIhvcNAQELBQADggEBAGHXWtzsmM7SubLXJZ8P0kbGYo9vNuhS/Z/wYeq6
UNe9lPiCttZjGNU5iXsoGMpiourb2BIw2yhBS9UOOFa/7FsyqGIVtCnpOkNapVtv
89tTww8Q7MQ3O6fWCGGh7hjJqAD+Yf5pAg1FXiAMANR19+CH9Qp8mdkykp2q8j09
ZXqSEWnizWeiI6u1X24ekbCVzomD8MGLBcZ76peG9m7BJIIxe7ktBFikhZNjid6c
RcOYZfZtRBWpdBBO2RN6h7jjJcL1sF8LY00rIdmwTny6Gm0NIropnqMC2suL7lj5
AQAFOEF4yrKbu/HxNpjHKcoOIdFIzrjJZnpiucgXoU8/Ous=
-----END CERTIFICATE-----
)EOF";


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
  printByte(message, length);
  Serial.println();
  char tempDistance[5];
  char tempEncryption[5];
  byte messageToEncrypt[12];
  dtostrf(distance, 5, 2, tempDistance);                            //Puts distance and decryption time in byte array
  memcpy(messageToEncrypt, tempDistance, strlen(tempDistance));
  dtostrf(ESP.getFreeHeap(), 6, 0, tempEncryption);
  messageToEncrypt[5] = '#';
  memcpy(messageToEncrypt + 6, tempEncryption, strlen(tempEncryption));
  printByte(messageToEncrypt, sizeof(messageToEncrypt));
  Serial.println();
  MQTTClient.publish("/test/reciever", messageToEncrypt, sizeof(messageToEncrypt), false);         //Sends MQTT message
}

void syncTime() {
    configTime(gmt_offset_sec, daylight_offset_sec, ntp_server);
    Serial.print("Waiting for NTP time sync: ");
    while (time(nullptr) < 8 * 3600 * 2) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Time synchronized");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        Serial.print("Current time: ");
        Serial.println(asctime(&timeinfo));
    } else {
        Serial.println("Failed to obtain local time");
    }
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

  syncTime();

  MQTTClient.setServer("192.168.1.113", 8883);            //MQTT settings setup
  MQTTClient.setCallback(recieve);

  BearSSL::X509List serverTrustedCA(ca_cert);
  
  wifiClient.setTrustAnchors(&serverTrustedCA);
  
  if(MQTTClient.connect("Reciever")) {                    //Connect to MQTT broker
    Serial.println("Connected to Broker");    
  } else {
    Serial.println("Problem connecting to Broker");
    char err_buf[128];
    wifiClient.getLastSSLError(err_buf, sizeof(err_buf));
    Serial.print("Failed to connect to MQTT broker, rc=");
    Serial.println(MQTTClient.state());
    Serial.print("SSL error: ");
    Serial.println(err_buf);
    delay(5000);
  }

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
