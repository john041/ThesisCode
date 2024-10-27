#include <MemoryUsage.h>
#include <Crypto.h>
#include <Ascon128.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[16];
char message[] = "SendDistAndTimes";
byte encryptedData[16];
byte decryptedData[16];
byte tag[16];
bool authenticated;
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

//Sets the key, IV, and counter for ChaCha
void setUpAscon(const unsigned char* IV) {
  ascon128.setKey(keyMemory,16);
  ascon128.setIV(IV, 16);
  ascon128.addAuthData(password, 8);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  convertFromString(key, keyMemory);
  printByte(keyMemory, sizeof(keyMemory));
  
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
    byte IV[16] = {(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32),(rand()%(127-32+1)+32)};

 
    delay(1000);
    startTime = micros();
    setUpAscon(IV);
    ascon128.encrypt(encryptedData, messageMemory, sizeof(messageMemory));
    totalTime = micros() - startTime;
    ascon128.computeTag(tag, sizeof(tag));
    Serial.print("#Encrypted Message#");
    printByte(encryptedData, sizeof(encryptedData));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    startTime = micros();
    setUpAscon(IV);
    ascon128.decrypt(decryptedData, encryptedData, sizeof(encryptedData));
    totalTime = micros() - startTime;
    authenticated = ascon128.checkTag(tag, sizeof(tag));
    Serial.print("#Decrypted Message#");
    printByte(decryptedData, sizeof(decryptedData));
    Serial.print("#DecryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
    Serial.print("#Authenticated#");
    if(authenticated){ Serial.print("True"); } else { Serial.print("False"); }
    Serial.println("");
  }
}
