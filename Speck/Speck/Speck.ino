#include <MemoryUsage.h>
#include <Crypto.h>
#include <Speck.h>

byte keyMemory[32];
char key[] = "Thisisatestaaaa!Thisisatestaaaa!";
byte messageMemory[16];
char message[] = "SendDistAndTimes";
byte encryptedData[16];
byte decryptedData[16];
float startTime;
float totalTime;
Speck speck;
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
  speck.setKey(keyMemory, sizeof(keyMemory));
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

    delay(1000);
    startTime = micros();
    for(int i = 0; i < sizeof(messageMemory); i += 16) {
      speck.encryptBlock(encryptedData + i, messageMemory + i);
    }
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    printByte(encryptedData, sizeof(encryptedData));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    startTime = micros();
    for(int i = 0; i < sizeof(encryptedData); i += 16) {
      speck.decryptBlock(decryptedData + i, encryptedData + i);
    }
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
