#include <Blowfish.h>
#include <String.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa";
byte messageMemory[16];
char message[] = "Encryptthi";
byte encryptedData[16];
byte decryptedData[16];
CBlowFish blowfishObject;

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
  
  blowfishObject.Initialize(keyMemory,16);

  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));

  float startTime = micros();
  blowfishObject.Encode(messageMemory, encryptedData, 16);  
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  blowfishObject.Decode(encryptedData, decryptedData, 16);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
