#include <SoftwareSerial.h>
#include <dht.h>

#define dht_pin 5
#define ldr_pin A0

SoftwareSerial SerialX(10,11);
dht DHT;
String Rx0,Rx1,Temp;

String Med_Temp(){
  DHT.read11(dht_pin);
  int temp = DHT.temperature;
  return String(temp);
}

String Med_Lum(){
  int lum = analogRead(A0);
  return String(lum);
}

void setup() {
  Serial.begin(9600);
  SerialX.begin(9600);  
}
 
void loop() {
  if(Serial.available()){
    Rx0 = Serial.readStringUntil('\n');    
    SerialX.println(Rx0);
  }
  if(SerialX.available()){
    Rx1 = SerialX.readStringUntil('\n');
    if(Rx1[0] == 'T'){
      SerialX.println("t" + Med_Temp());
    }

    if(Rx1[0] == 'L'){
      SerialX.println("l" + Med_Lum());
    }

    if(Rx1[0] == 't'){
      Serial.println(Rx1);
    }

    if(Rx1[0] == 'l'){
      Serial.println(Rx1);
    }

    if(Rx1[0] == 'C'){
      Serial.println(Rx1);
    }
  }
}
