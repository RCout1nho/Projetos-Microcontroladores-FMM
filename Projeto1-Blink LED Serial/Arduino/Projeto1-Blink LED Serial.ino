#define led 13

int x = 0;

void setup() {
  pinMode(led,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    x = Serial.parseInt();
    
    Serial.read();

    digitalWrite(led,1);

    delay(x*200);

    digitalWrite(led,0);

    delay(x*200);
  }else{
    digitalWrite(led,1);

    delay(x*200);

    digitalWrite(led,0);

    delay(x*200); 
  }
}
