#include <MemoryUsage.h>
#include <tinyECC.h> 

float startTime;
float totalTime;
tinyECC tinyEcc;
int j = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(j < 200) {
    j++;
    Serial.print("Number#");
    Serial.print(j);
    
    delay(1000);
    tinyEcc.plaintext = "This is a long sentence that is encrypted and then transmitted using the MQTT protocol for test.";
    startTime = micros();
    tinyEcc.encrypt();  
    totalTime = micros() - startTime;
    Serial.print("#Encrypted Message#");
    Serial.print(tinyEcc.ciphertext);
    Serial.print("#EncryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;

    delay(1000);
    startTime = micros();
    tinyEcc.decrypt();  
    totalTime = micros() - startTime;
    Serial.print("#Decrypted Message#");
    Serial.print(tinyEcc.plaintext);
    Serial.print("#DecryptTime#");
    Serial.print(totalTime);
    Serial.print("#");
    MEMORY_PRINT_FREERAM;
    Serial.println("");
  }
}
