#include <xxtea-iot-crypt.h>

char key[] = "Thisisatestaaaav";
char message[] = "Encryptthi";
String encryptedData;
String decryptedData;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  xxtea.setKey(key);
  Serial.println(key);
  Serial.println(message);

  float startTime = micros();
  encryptedData = xxtea.encrypt(message);
  float totalTime = micros() - startTime;
  Serial.println(encryptedData);
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);


  startTime = micros();
  decryptedData = xxtea.decrypt(encryptedData);
  totalTime = micros() - startTime;
  Serial.println(decryptedData);
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
