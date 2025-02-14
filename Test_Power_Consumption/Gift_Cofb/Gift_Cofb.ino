#include <gift-cofb-aead.h>

byte keyMemory[16];
char key[] = "Thisisatestaaaa!";
byte messageMemory[1072];
char message[] = "The data that is collected will come from two sources. The first source is from the serial output of the code, including the measurements of round-trip time, encryption execution time, decryption time, and free memory. These measurements are output by the serial output to a screen in the Arduino IDE for each packet that is sent. In each test case, 200 packets are sent with each of these measurements. Once the measurements are captured, an average of 200 samples are taken for each measurement. There are a couple of samples that are taken multiple times for the packet. The free memory measurement is taken three times throughout the code to see what the free memory is at various points in the encryption process. These three samples are averaged out of the 200 packets, and the lowest average is taken as the final measurement. Two decryption execution times are also sampled, one from the sending device and once from the receiving device. The decryption execution time from the receiving device is used as it is the decryption for the original message.123456789012";
byte encryptedData[1088];
byte decryptedData[1072];
byte authMemory[16];
int authenticated;
char auth[] = "passwordpassword";
int j = 0;
byte IV[16] = {(rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32), (rand()%(127-32+1)+32)};


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

  ESP.wdtDisable();                                 //Disable software watchdog
  *((volatile uint32_t*) 0x60000900) &= ~(1);       //Disable hardware watchdog
  
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
  Serial.println("While loop starts");
  while(j == 0) {

    size_t sizeOfEncrypted = sizeof(encryptedData);
    gift_cofb_aead_encrypt(encryptedData, &sizeOfEncrypted, messageMemory, sizeof(messageMemory), authMemory, 16, IV, keyMemory);  
    //Serial.print("#Encrypted Message#");
    //printByte(encryptedData, sizeof(encryptedData));
    //Serial.println("#");

    size_t sizeOfDecrypted = sizeof(decryptedData);
    authenticated = gift_cofb_aead_decrypt(decryptedData, &sizeOfDecrypted, encryptedData, sizeof(encryptedData), authMemory, 16, IV, keyMemory);
    //Serial.print("#Decrypted Message#");
    //printByte(decryptedData, sizeof(decryptedData));
    //Serial.print("#Authenticated#");
    //if(authenticated == 0){ Serial.print("True"); } else { Serial.print("False"); }
    //Serial.println("");
  }
  Serial.println("Error");
}
