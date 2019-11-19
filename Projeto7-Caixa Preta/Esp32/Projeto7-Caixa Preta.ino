#include <WiFi.h>
#include <FirebaseESP32.h>

FirebaseData firebaseData;

#define potVel 33
#define potAlt 34
#define SwTrem 32

String basepath = "https://projeto-parcial2.firebaseio.com/";
String authsecret = "NZ8XagMJtYlhQcC1837fZXFy9C57wXiyIVe1FkuU";

char ssid[] = "Galaxy J8";
char password[] = "senha12345";

int ir;

void setup() {
    Serial.begin(115200);
    pinMode(potVel,INPUT);
    pinMode(potAlt,INPUT);
    pinMode(SwTrem,INPUT_PULLUP);
    
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
      Serial.println(".");
      delay(500);
    }
    Serial.println("Conectado");
    Firebase.begin(basepath, authsecret);
    Firebase.reconnectWiFi(true);
}

void loop() {
    delay(2500);
    int ValVelocidade = analogRead(potVel);
    int ValAltitude = analogRead(potAlt);
    int EstadoTrem = digitalRead(SwTrem);

    Firebase.getInt(firebaseData, "/Decolagem/Indice/Registro");
    ir = firebaseData.intData();
    Firebase.setString(firebaseData, "/Decolagem/Registro/"+String(ir+1)+"/Altitude", String(ValAltitude));
    Firebase.setString(firebaseData, "/Decolagem/Registro/"+String(ir+1)+"/Velocidade", String(ValVelocidade));
    Firebase.setString(firebaseData, "/Decolagem/Registro/"+String(ir+1)+"/Trem", String(EstadoTrem));

    Firebase.setInt(firebaseData, "/Decolagem/Indice/Registro", ir+1);
}
