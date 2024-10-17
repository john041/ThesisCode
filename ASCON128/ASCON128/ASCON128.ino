#include <Crypto.h>
#include <Ascon128.h>
#include <string.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[32];
char message[] = "SendDistAndTimesSendDistAndTimes";
byte encryptedData[32];
byte decryptedData[32];
byte tag[16];
byte IV[] = { 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,0x0a};
const char password[] = "password";


Ascon128 ascon128;

void convertFromString( const char* info, byte* memory ) {
  for( int i = 0; i < strlen(info); i++ ) {
    memory[i] = (byte)info[i];
  }
}

void printByte( byte* info, int sizeOfArray) {
  for( int i = 0; i < sizeOfArray; i++ ) {
    Serial.write(info[i]);
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  convertFromString(key, keyMemory);
  printByte(keyMemory, sizeof(keyMemory));
  
  ascon128.setKey(keyMemory, 16);
  ascon128.setIV(IV, 16);
  //ascon128.addAuthData(password, 8);
  

  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));


  float startTime = micros();
  ascon128.encrypt(encryptedData, messageMemory, 32);
  //ascon128.computeTag(tag, 8);  
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  //ascon128.clear();

  ascon128.setKey(keyMemory,16);
  ascon128.setIV(IV, 16);
  //ascon128.addAuthData(password, 8);

  startTime = micros();
  ascon128.decrypt(decryptedData, encryptedData, 32);
  //ascon128.checkTag(tag, 8);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:
}
