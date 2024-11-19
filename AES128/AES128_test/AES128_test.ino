#include <MemoryUsage.h>
#include <Crypto.h>
#include <AES.h> 

char keyMemory[32] = {'T','h','i','s','i','s','a','t','e','s','t','a','a','a','a','!','T','h','i','s','i','s','a','t','e','s','t','a','a','a','a','!'};
byte messageMemory[96] = {'T','h','i','s',' ','i','s',' ','a',' ','l','o','n','g',' ','s','e','n','t','e','n','c','e',' ','t','h','a','t',' ','i','s',' ','e','n','c','r','y','p','t','e','d',' ','a','n','d',' ','t','h','e','n',' ','t','r','a','n','s','m','i','t','t','e','d',' ','u','s','i','n','g',' ','t','h','e',' ','M','Q','T','T',' ','p','r','o','t','o','c','o','l',' ','f','o','r',' ','t','e','s','t','.'};
byte encryptedData[96];
byte decryptedData[96];
//float startTime;
//float totalTime;
//AES128 aes;
AES256 aes;
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
  
  //convertFromString(key, keyMemory);
  printByte(keyMemory, sizeof(keyMemory));
  aes.setKey(keyMemory, sizeof(keyMemory));
  //convertFromString(message, messageMemory);
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
    float startTime = micros();
    for(int i = 0; i < sizeof(messageMemory); i += 16) {
     aes.encryptBlock(encryptedData + i, messageMemory + i);                     //Encrypt the char array
    }
    float totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    //printByte(encryptedData, sizeof(encryptedData));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    startTime = micros();
    for(int i = 0; i < sizeof(encryptedData); i += 16) {
     aes.decryptBlock(decryptedData + i, encryptedData + i);                     //Encrypt the char array
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
