//#include <MemoryUsage.h>
#include <xxtea-iot-crypt.h>


byte key[] = "Thisisatestaaaa!";
char message[] = "The data that is collected will come from two sources. The first source is from the serial output of the code, including the measurements of round-trip time, encryption execution time, decryption time, and free memory. These measurements are output by the serial output to a screen in the Arduino IDE for each packet that is sent. In each test case, 200 packets are sent with each of these measurements. Once the measurements are captured, an average of 200 samples are taken for each measurement. There are a couple of samples that are taken multiple times for the packet. The free memory measurement is taken three times throughout the code to see what the free memory is at various points in the encryption process. These three samples are averaged out of the 200 packets, and the lowest average is taken as the final measurement. Two decryption execution times are also sampled, one from the sending device and once from the receiving device. The decryption execution time from the receiving device is used as it is the decryption for the original message.";
byte messageMemory[1060];
byte dataBuffer[1060];
int num = 0;
float startTime;
float totalTime;
int j = 0;
int32_t len = 1060;

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
    //j++;
    //Serial.print("Number#");
    //Serial.print(j);

    //delay(1000);
    startTime = micros();
    xxtea_encrypt(messageMemory, num, dataBuffer, &len);
    totalTime = micros() - startTime;
    /*num = sizeof(messageMemory);
    startTime = micros();
    for(int i = 0; i < sizeof(messageMemory); i += 80) {
       len = 96;
       if(num > 80){
          int temp  = xxtea_encrypt(messageMemory + i, 80, dataBuffer + i, &len);                                //Encrypt the char array
          num = num - 80;
       } else {
          xxtea_encrypt(messageMemory + i, num, dataBuffer + i, &len);
       }
     }
    totalTime = micros() - startTime;*/
    //Serial.print("#Encrypted Message#");
    //printByte(dataBuffer, sizeof(dataBuffer));
    //Serial.print("#EncryptTime#");
    //Serial.print(totalTime);
    //Serial.print("#");
    //MEMORY_PRINT_FREERAM;
  
    //delay(1000);
    startTime = micros();
    xxtea_decrypt(dataBuffer, num);
    totalTime = micros() - startTime;
    /*num = sizeof(dataBuffer);
    startTime = micros();
    for(int i = 0; i < sizeof(dataBuffer); i += 80) {
       if(num > 80){
         xxtea_decrypt(dataBuffer + i, 80);                     //Encrypt the char array
         num = num - 80;
       } else {
         xxtea_decrypt(dataBuffer + i, num);
       }
     }
    totalTime = micros() - startTime;*/
    //Serial.print("#Decrypted Message#");
    //printByte(dataBuffer, sizeof(dataBuffer));
    //Serial.print("#DecryptTime#");
    //Serial.print(totalTime);
    //Serial.print("#");
    //MEMORY_PRINT_FREERAM;
    //Serial.println("");
  }
}
