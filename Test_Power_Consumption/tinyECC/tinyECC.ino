#include <tinyECC.h> 

float startTime;
float totalTime;
tinyECC tinyEcc;
int j = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  ESP.wdtDisable();                                 //Disable software watchdog
  *((volatile uint32_t*) 0x60000900) &= ~(1);       //Disable hardware watchdog
  
  delay(5000);
}

void loop() {
  Serial.println("While loop starts");
  while(j == 0) {

    tinyEcc.plaintext = "The data that is collected will come from two sources. The first source is from the serial";
    tinyEcc.encrypt();  
    //Serial.print("#Encrypted Message#");
    //Serial.print(tinyEcc.ciphertext);
    //Serial.println("#");

    tinyEcc.decrypt();
    //Serial.print("#Decrypted Message#");
    //Serial.print(tinyEcc.plaintext);
    //Serial.println("");
  }
  Serial.println("Error");
}
