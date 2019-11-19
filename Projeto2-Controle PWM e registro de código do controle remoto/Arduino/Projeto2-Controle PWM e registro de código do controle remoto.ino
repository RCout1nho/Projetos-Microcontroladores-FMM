#include <IRremote.h>

#define SINAL_RECEBIDO_REM 46
#define LED 8

IRrecv RECEPTOR(SINAL_RECEBIDO_REM);
decode_results SINAL_ENVIADO_CONT;

long int BOT = 0;
long int BOT2 = 0;
bool COND = false;
bool a = false;
bool aux = false;
int aux2 = 0;

void setup() {
Serial.begin(9600);
pinMode(SINAL_RECEBIDO_REM,INPUT);
pinMode(LED, OUTPUT);
RECEPTOR.enableIRIn();
}
void loop()
{
if (RECEPTOR.decode(&SINAL_ENVIADO_CONT))
{
    BOT = SINAL_ENVIADO_CONT.value;
 
    if(BOT == 0xFF42BD)
    {
        Serial.println("ON/OFF");
        if(a){
          a=false;
          analogWrite(LED,0);
        }
        else{
          a = true;          
        }          
    }
    if(a == true)
    {
        if(BOT != 0xFF42BD && BOT!=0XFFFFFFFF) BOT2=BOT;         
        switch(BOT2){
            case 0xFF4AB5:
                Serial.println("DEC:0 HEX:0xFF4AB5");
                analogWrite(LED, 30);
                break;
            case 0xFF6897:
                Serial.println("DEC:1 HEX:0xFF6897");
                analogWrite(LED, 55);
                break;
            case 0xFF9867:
                Serial.println("DEC:2 HEX:0xFF9867");
                analogWrite(LED, 80);
                break;
            case 0xFFB04F:
                Serial.println("DEC:3 HEX:0xFFB04F");
                analogWrite(LED, 105);
                break;
            case 0xFF30CF:
                Serial.println("DEC:4 HEX:0xFF30CF");
                analogWrite(LED, 130);
                break;
            case 0xFF18E7:
                Serial.println("DEC:5 HEX:0xFF18E7");
                analogWrite(LED, 155);
                break;
            case 0xFF7A85:
                Serial.println("DEC:6 HEX:0xFF7A85");
                analogWrite(LED, 180);
                break;
            case 0xFF10EF:
                Serial.println("DEC:7 HEX:0xFF10EF");
                analogWrite(LED, 205);
                break;
            case 0xFF38C7:
                Serial.println("DEC:8 HEX:0xFF38C7");
                analogWrite(LED, 230);
                break;
            case 0xFF5AA5:
                Serial.println("DEC:9 HEX:0xFF5AA5");
                analogWrite(LED, 255);
                break;
            default:
                break;      
        }
    }
    delay(500);
    RECEPTOR.resume();
  }
}
