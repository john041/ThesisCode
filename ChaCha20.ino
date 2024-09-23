#include <Crypto.h>
#include <ChaCha.h>
#include <string.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaav";
byte messageMemory[16];
char message[] = "Encryptthi";
byte encryptedData[16];
byte decryptedData[16];
byte IV[] = { 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a};
byte counter[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
ChaCha chacha20;

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
  
  Serial.println((bool)chacha20.setKey(keyMemory,16));
  Serial.println((bool)chacha20.setIV(IV, 8));
  Serial.println((bool)chacha20.setCounter(counter, 8));
  chacha20.setNumRounds(20);
  

  


  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));


  float startTime = micros();
  chacha20.encrypt(encryptedData, messageMemory, 10);  
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);


  chacha20.clear();

  Serial.println((bool)chacha20.setKey(keyMemory,16));
  Serial.println((bool)chacha20.setIV(IV, 8));
  Serial.println((bool)chacha20.setCounter(counter, 8));
  chacha20.setNumRounds(20);

  startTime = micros();
  chacha20.decrypt(decryptedData, encryptedData, 10);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
