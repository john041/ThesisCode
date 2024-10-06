#include <xxtea-iot-crypt.h>

byte key[] = "Thisisatestaaaa!";
byte message[] = "SendDistAndTimes";
byte encryptedData[32];
byte decryptedData[16];
int len = 32;

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
  Serial.println();
  printByte(message, strlen((char*)message));
  Serial.println();

  float startTime = micros();
  xxtea_encrypt(message, strlen((char*)message), encryptedData, &len);
  float totalTime = micros() - startTime;
  printByte(encryptedData, strlen((char*)encryptedData));
  Serial.println();
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);


  startTime = micros();
  xxtea_decrypt(encryptedData, len);
  totalTime = micros() - startTime;
  printByte(encryptedData, strlen((char*)encryptedData));
  Serial.println();
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
