#include <DES.h>
#include <String.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa";
byte messageMemory[16];
char message[] = "Encryptt";
byte encryptedData[16];
byte decryptedData[16];
DES des;

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


  float startTime = micros();
  des.encrypt(encryptedData, messageMemory, keyMemory);  
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  des.decrypt(decryptedData, encryptedData, keyMemory);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:
}
