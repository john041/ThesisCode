#include <MemoryUsage.h>
#include <Crypto.h>
#include <ChaCha.h>

byte keyMemory[32];
char key[] = "Thisisatestaaaa!Thisisatestaaaa!";
byte messageMemory[16];
char message[] = "SendDistAndTimes";
byte encryptedData[16];
byte decryptedData[16];
float startTime;
float totalTime;
ChaCha chacha20;
int j = 0;

void convertFromString( const char* info, byte* memory ) {
  for( int i = 0; i < strlen(info); i++ ) {
    memory[i] = (byte)info[i];
  }
}

void printByte( byte* info, int sizeOfArray) {
  for( int i = 0; i < sizeOfArray; i++ ) {
    Serial.write(info[i]);
  }
}

void setUpChaCha20(const unsigned char* IV, const unsigned char* counter) {
  chacha20.setKey(keyMemory,16);
  chacha20.setIV(IV, 8);
  chacha20.setCounter(counter, 8);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  convertFromString(key, keyMemory);
  printByte(keyMemory, sizeof(keyMemory));
  chacha20.setNumRounds(20);
  
  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));
  Serial.println();
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(j < 200) {
    j++;
    Serial.print("Number#");
    Serial.print(j);
    byte IV[8] = {(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32)};
    byte counter[8] = {(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32)};
    
    delay(1000);
    startTime = micros();
    setUpChaCha20(IV, counter);
    chacha20.encrypt(encryptedData, messageMemory, sizeof(messageMemory));
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    //printByte(encryptedData, sizeof(encryptedData));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    startTime = micros();
    setUpChaCha20(IV, counter);
    chacha20.decrypt(decryptedData, encryptedData, sizeof(encryptedData));
    totalTime = micros() - startTime;
    Serial.print("#Decrypted Message#");
    printByte(decryptedData, sizeof(decryptedData));
    Serial.print("#DecryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
    Serial.println("");
  }
}
