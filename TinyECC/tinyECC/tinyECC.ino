#include <tinyECC.h> 

tinyECC tinyEcc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  tinyEcc.plaintext = "Encryptthi";
  float startTime = micros();
  tinyEcc.encrypt();  
  float totalTime = micros() - startTime;
  Serial.println(tinyEcc.ciphertext);
  Serial.print("Total time to encrypt is ");
  Serial.println(totalTime);

  startTime = micros();
  tinyEcc.decrypt();  
  totalTime = micros() - startTime;
  Serial.println(tinyEcc.plaintext);
  Serial.print("Total time to decrypt is ");
  Serial.println(totalTime);
}

void loop() {
  // put your main code here, to run repeatedly:

}
