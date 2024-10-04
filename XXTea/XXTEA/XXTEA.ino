#include <xxtea-iot-crypt.h>

char key[] = "Thisisatestaaaa!";
char message[] = "SendDistAndTimes";
char encryptedData[33];
char decryptedData[16];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xxtea.setKey(key);
  Serial.println(key);
  Serial.println(message);

  float startTime = micros();
  strcpy(encryptedData, (xxtea.encrypt(message)).c_str());
  float totalTime = micros() - startTime;
  Serial.println(strlen(encryptedData));
  Serial.println(encryptedData);
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);


  startTime = micros();
  strcpy(decryptedData, (xxtea.decrypt(encryptedData)).c_str());
  totalTime = micros() - startTime;
  Serial.println(decryptedData);
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
