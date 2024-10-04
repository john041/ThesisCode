#include <Blowfish.h>
#include <String.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[1048];
char message[] = "This is super long to test how well thisdoes and if it is a block cifer";
byte encryptedData[1048];
byte decryptedData[1048];
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
  printByte(keyMemory, strlen((char*)keyMemory));
  
  blowfishObject.Initialize(keyMemory,16);

  convertFromString(message, messageMemory);
  printByte(messageMemory, strlen((char*)messageMemory));

  float startTime = micros();
  blowfishObject.Encode(messageMemory, encryptedData, strlen((char*)messageMemory));  
  float totalTime = micros() - startTime;
  printByte(encryptedData, strlen((char*)encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  blowfishObject.Decode(encryptedData, decryptedData, strlen((char*)encryptedData));
  totalTime = micros() - startTime;
  printByte(decryptedData, strlen((char*)decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
