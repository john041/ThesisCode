#include <Crypto.h>
#include <Speck.h>
#include <string.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaaav";
byte messageMemory[16];
char message[] = "Encryptthi";
byte encryptedData[16];
byte decryptedData[16];
Speck speck;

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
  
  speck.setKey(keyMemory,32);

  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));


  float startTime = micros();
  speck.encryptBlock(encryptedData, messageMemory);  
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  speck.decryptBlock(decryptedData, encryptedData);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:
}
