#include <MemoryUsage.h>
#include <romulus-n-aead.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[16];
char message[] = "SendDistAndTimes";
byte encryptedData[32];
byte decryptedData[16];
byte authMemory[16];
int authenticated;
char auth[] = "passwordpassword";
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
  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));
  
  convertFromString(auth, authMemory);
  printByte(authMemory, sizeof(authMemory));
  Serial.println();
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(j < 200) {
    j++;
    Serial.print("Number#");
    Serial.print(j);
    byte IV[16] = {(rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32)};

    delay(1000);
    size_t sizeOfEncrypted = sizeof(encryptedData);
    startTime = micros();
    romulus_n_aead_encrypt(encryptedData, &sizeOfEncrypted, messageMemory, sizeof(messageMemory), authMemory, 16, IV, keyMemory);  
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    printByte(encryptedData, sizeof(encryptedData));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    size_t sizeOfDecrypted = sizeof(decryptedData);
    startTime = micros();
    authenticated = romulus_n_aead_decrypt(decryptedData, &sizeOfDecrypted, encryptedData, sizeof(encryptedData), authMemory, 16, IV, keyMemory);
    totalTime = micros() - startTime;
    Serial.print("#Decrypted Message#");
    printByte(decryptedData, sizeof(decryptedData));
    Serial.print("#DecryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
    Serial.print("#Authenticated#");
    if(authenticated == 0){ Serial.print("True"); } else { Serial.print("False"); }
    Serial.println("");
  }
}
