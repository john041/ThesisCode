#include <MemoryUsage.h>
#include <Crypto.h>
#include <Ascon128.h>
#include <string.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[16];
char message[] = "SendDistAndTimes";
byte encryptedData[24];
byte decryptedData[16];
byte tag[16];
byte IV[] = { 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a,0x0a};
const char password[] = "password";
Ascon128 ascon128;
float startTime;
float totalTime;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  convertFromString(key, keyMemory);
  printByte(keyMemory, sizeof(keyMemory));
  
  ascon128.setKey(keyMemory, 16);
  ascon128.setIV(IV, 16);
  ascon128.addAuthData(password, 8);
  
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

    ascon128.setKey(keyMemory,16);
    ascon128.setIV(IV, 16);
    ascon128.addAuthData(password, 8);

    startTime = micros();
    ascon128.encrypt(encryptedData, messageMemory, sizeof(messageMemory));
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    printByte(encryptedData, sizeof(encryptedData));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    ascon128.setKey(keyMemory,16);
    ascon128.setIV(IV, 16);
    ascon128.addAuthData(password, 8);
    
    startTime = micros();
    ascon128.decrypt(decryptedData, encryptedData, 16);
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
