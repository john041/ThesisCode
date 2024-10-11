#include <Crypto.h>
#include <ChaCha.h>
#include <string.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[72];
char message[] = "This is super long to test how well thisdoes and if it is a block cifer";
byte encryptedData[72];
byte decryptedData[72];
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

  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));

  chacha20.setNumRounds(20);

  for(int i = 0; i < 5; i++) {
    byte IV[8] = {(rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10))};
    byte counter[8] = {(rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10)), (rand() % (10))};
    //printByte(IV, 8);
    chacha20.setKey(keyMemory,16);
    chacha20.setIV(IV, 8);
    chacha20.setCounter(counter, 8);
    
    float startTime = micros();
    chacha20.encrypt(encryptedData, messageMemory, 72);  
    float totalTime = micros() - startTime;
    printByte(encryptedData, sizeof(encryptedData));
    Serial.print("Total time to encrypt is ");
    Serial.println(totalTime);
    
    
    chacha20.setKey(keyMemory,16);
    chacha20.setIV(IV, 8);
    chacha20.setCounter(counter, 8);
    
    startTime = micros();
    chacha20.decrypt(decryptedData, encryptedData, 72);
    totalTime = micros() - startTime;
    printByte(decryptedData, sizeof(decryptedData));
    Serial.print("Total time to decrypt is ");
    Serial.println(totalTime);
    Serial.print(ESP.getFreeHeap());
    Serial.println("_____________________________________________");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
