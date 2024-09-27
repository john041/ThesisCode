#include <Crypto.h>
#include <AES.h> 
#include <string.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[16];
char message[] = "Encryptthing!!!!";
byte encryptedData[16];
byte decryptedData[16];
AES128 aes;

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

  aes.setKey(keyMemory, 16);
   
  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));

  float startTime = micros();
  aes.encryptBlock(encryptedData, messageMemory);
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  aes.decryptBlock(decryptedData, encryptedData);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);

  Serial.println(heap_caps_get_free_size());
  Serial.println(uxTaskGetStackHighWaterMark(NULL));
}

void loop() {
  // put your main code here, to run repeatedly:
}
