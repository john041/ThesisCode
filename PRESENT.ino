#include <present.h>
#include <string.h>

byte keyMemory[16];
char key[] = "Testthis!!";
byte messageMemory[16];
char message[] = "Encryptt";

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
  present_encrypt(messageMemory, keyMemory);  
  float totalTime = micros() - startTime;
  printByte(messageMemory, sizeof(messageMemory));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  present_decrypt(messageMemory, keyMemory);
  totalTime = micros() - startTime;
  printByte(messageMemory, sizeof(messageMemory));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:
}
