//---Bibliotecas---
#include <Time.h>
#include <FirebaseESP32.h>
#include <WiFi.h>

//**************************************
//---Pinos---
#define ldr_pin 33
#define pot_pin 34
#define led_pin 4

#define btn1_pin 5
#define btn2_pin 18

//**************************************
//---CONSTANTES---
//---WiFi---
const char* ssid = "Galaxy J8";
const char* password = "senha12345";

//---Firebase---
const char* firebase_host = "https://projeto-parcial1x.firebaseio.com/";
const char* firebase_auth = "AaHaWVLuMRlT3wJwX7PCXDMmgFKw14M8KFqk07BS";

//---Timer/NTP server---
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = -14400;

//**************************************
//---Objetos---
FirebaseData firebaseData;

//**************************************
//---Variaveis Globais---
int il,ip;

//**************************************
//---Protótipos das funcões---
String getLocalTime();
String getLocalDate();
String getLdrValue();
String getPotValue();
bool btn_click();

//**************************************
void setup() {
  pinMode(ldr_pin,INPUT); 
  pinMode(pot_pin,INPUT);
  pinMode(btn1_pin,INPUT_PULLUP);
  pinMode(btn2_pin,INPUT_PULLUP);
  
  Serial.begin(115200);

  WiFi.begin(ssid,password);
  Serial.print("Conectando ao WiFi");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(firebase_host, firebase_auth);
  Firebase.reconnectWiFi(true);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

//**************************************
void loop() {  
  //**************************************
  //---Leitura dos botões---
  if(btn_click("btn1")){
    Serial.println("Btn1 Pressionado");
    Firebase.setBool(firebaseData,"/Solicitacoes/LDR", true);
  }
  if(btn_click("btn2")){
    Serial.println("Btn2 Pressionado");
    Firebase.setBool(firebaseData,"/Solicitacoes/Pot", true);
  }
  
  //---Verifica se há alguma solicitação---
  Firebase.getBool(firebaseData,"/Solicitacoes/LDR");
  bool Sldr = firebaseData.boolData();

  Firebase.getBool(firebaseData,"/Solicitacoes/Pot");
  bool Spot = firebaseData.boolData();
  //**************************************

  Serial.println(firebaseData.jsonData());        

  //---Solicitação de LDR---
  if(Sldr){
    Serial.println("Lendo LDR...");
    delay(2000);
    Firebase.getInt(firebaseData,"/Indexes/LDR");
    il = firebaseData.intData();
    Serial.print("LDR--->");Serial.println(il);
    Firebase.setString(firebaseData, "/Sensores/LDR/"+String(il+1)+"/Hora", getLocalTime());
    Firebase.setString(firebaseData, "/Sensores/LDR/"+String(il+1)+"/Data", getLocalDate());
    Firebase.setString(firebaseData, "/Sensores/LDR/"+String(il+1)+"/Valor", getLdrValue());

    Firebase.setBool(firebaseData, "/Solicitacoes/LDR", false);    
    Firebase.setInt(firebaseData, "/Indexes/LDR", il+1);
    Firebase.setBool(firebaseData, "/Respostas/LDR", true);
  }

  //---Solicitação de Pot----
  if(Spot){
    Serial.println("Lendo Potenciometro...");
    delay(2000);
    Firebase.getInt(firebaseData,"/Indexes/Pot");
    ip = firebaseData.intData();
    Serial.print("POT--->");Serial.println(ip);   
    Firebase.setString(firebaseData, "/Sensores/Pot/"+String(ip+1)+"/Hora", getLocalTime());
    Firebase.setString(firebaseData, "/Sensores/Pot/"+String(ip+1)+"/Data", getLocalDate());
    Firebase.setString(firebaseData, "/Sensores/Pot/"+String(ip+1)+"/Valor", getPotValue());

    Firebase.setBool(firebaseData, "/Solicitacoes/Pot", false);
    Firebase.setInt(firebaseData, "/Indexes/Pot", ip+1);
    Firebase.setBool(firebaseData, "/Respostas/Pot", true);    
  }
}

//**************************************
//---Funções auxiliares--
String getLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha ao obter hora");
    return "Error";
  }
    
  String Time = String(timeinfo.tm_hour)+':'+String(timeinfo.tm_min)+':'+String(timeinfo.tm_sec);
  return Time;
}

String getLocalDate()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha ao obter data");
    return "Error";
  }

  String date = String(timeinfo.tm_mday)+'/'+String(timeinfo.tm_mon+1)+'/'+String(timeinfo.tm_year+1900);
  return date;
}

String getLdrValue()
{
  int ldr = analogRead(ldr_pin);
  Serial.print("LDR= ");Serial.println(ldr);
  return String(ldr);
}

String getPotValue()
{
  int pot = analogRead(pot_pin);
  Serial.print("Pot = ");Serial.println(pot);
  return String(pot);
}

bool btn_click(String btn)
{
  if(btn == "btn1")
  {
    if(!digitalRead(btn1_pin)){
      delay(500);
      return true;
    }
    return false;
  }

  if(btn == "btn2")
  {
    if(!digitalRead(btn2_pin)){
      delay(500);
      return true;
    }
    return false;
  }
}
