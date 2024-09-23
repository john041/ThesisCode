#include <xoodyak-aead.h>
#include <String.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaav";
byte messageMemory[16];
char message[] = "Encryptthi";
byte encryptedData[32];
byte decryptedData[32];
char auth[] = "passwordpassword";
byte authMemory[16];
byte IV[] = { 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a};

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

  convertFromString(auth, authMemory);
  printByte(authMemory, sizeof(authMemory));


  size_t sizeOfEncrypted = sizeof(encryptedData);
  float startTime = micros();
  xoodyak_aead_encrypt(encryptedData, &sizeOfEncrypted, messageMemory, 10, authMemory, 16, IV, keyMemory);  
  float totalTime = micros() - startTime;
  printByte(encryptedData, sizeof(encryptedData));
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);


  size_t sizeOfDecrypted = sizeof(decryptedData);
  startTime = micros();
  xoodyak_aead_decrypt(decryptedData, &sizeOfDecrypted, encryptedData, 26, authMemory, 16, IV, keyMemory);
  totalTime = micros() - startTime;
  printByte(decryptedData, sizeof(decryptedData));
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
