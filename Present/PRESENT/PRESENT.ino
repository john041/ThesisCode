#include <MemoryUsage.h>
#include <present.h>

char key[] = "Thisisatestaaaa!";
char message[] = "SendDistAndTimes";
byte keyMemory[16];
byte encryptedMemory[16];
byte decryptedMemory[16];
float startTime;
float totalTime;
int length = 0;
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
    convertFromString(message, encryptedMemory);
    length = sizeof(encryptedMemory);
    startTime = micros();
    for( int i = 0; i < length; i += 8) {
      present_encrypt(encryptedMemory + i, keyMemory);
    }
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    printByte(encryptedMemory, sizeof(encryptedMemory));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    memcpy(decryptedMemory, encryptedMemory, length);
    length = sizeof(decryptedMemory);
    startTime = micros();
    for( int i = 0; i < length; i += 8) {
      present_decrypt(decryptedMemory + i, keyMemory);
    }
    totalTime = micros() - startTime;
    Serial.print("#Decrypted Message#");
    printByte(decryptedMemory, sizeof(decryptedMemory));
    Serial.print("#DecryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
    Serial.println("");
  }
}
