#include <MemoryUsage.h>
#include <xxtea-iot-crypt.h>

byte key[] = "Thisisatestaaaa!";
byte message[] = "SendDistAndTimes";
byte messageMemory[16];
byte dataBuffer[16];
long int len = 16;
int num = 0;
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

  xxtea_setup(key, strlen((char*)key));
  printByte(key, strlen((char*)key));
  Serial.print(" ");
  convertFromString(message, messageMemory);
  printByte(messageMemory, sizeof(messageMemory));
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(j < 200) {
    j++;
    Serial.print("Number#");
    Serial.print(j);

    delay(1000);
    num = sizeof(messageMemory);
    startTime = micros();
    for(int i = 0; i < sizeof(messageMemory); i += 80) {
       if(num > 80){
          xxtea_encrypt(messageMemory + i, num % 80, dataBuffer + i, &len);                                //Encrypt the char array
          num = num - 80;
       } else {
          xxtea_encrypt(messageMemory + i, num, dataBuffer + i, &len);
       }
     }
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    printByte(dataBuffer, sizeof(dataBuffer));
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
  
    delay(1000);
    num = sizeof(dataBuffer);
    startTime = micros();
    for(int i = 0; i < sizeof(dataBuffer); i += 80) {
       if(num > 80){
         xxtea_decrypt(dataBuffer + i, num % 80);                     //Encrypt the char array
         num = num - 80;
       } else {
         xxtea_decrypt(dataBuffer + i, num);
       }
     }
    totalTime = micros() - startTime;
    Serial.print("#Decrypted Message#");
    printByte(dataBuffer, sizeof(dataBuffer));
    Serial.print("#DecryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
    Serial.println("");
  }
}
