//---Bibliotecas---
#include <LiquidCrystal_I2C.h>

//---Pinos---
#define btn_ok 8
#define btn_t1 9
#define btn_t2 10
#define btn_next 7
#define pin_Buzzer 3

//---Objetos---
LiquidCrystal_I2C lcd(0x27,16,2);

//**************************************
//---Opções do Menu---
String menu[2]= { "Futsal", "Domino"};

//--Variáveis auxiliares---
const int tempo = 10; 
int tempInit,tempAt,ts,tm,op=0,t1,t2;

//**************************************
void setup()
{
  pinMode(btn_ok,INPUT_PULLUP);
  pinMode(btn_t1,INPUT_PULLUP);
  pinMode(btn_t2,INPUT_PULLUP);
  pinMode(btn_next,INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.print("Micro- AV3");
}

//**************************************
void loop()
{
    if(btn_click(btn_ok))
    {
      switch(op)
      {
        //---Opção Futsal---
        case 0:
          tempInit = millis();//Tempo inicial
          
          t1=0;t2=0;
          
          lcd.clear();          
          lcd.setCursor(0,0);
          lcd.print("T1:" + String(t1));
          lcd.setCursor(0,1);
          lcd.print("T2:" + String(t2));
          
          delay(50);

          //---Placar do futsal + tempo---
          while(!btn_click(btn_ok))
          {
            //----Gol time 1---
            if(btn_click(btn_t1))
            {
              t1++; 
              lcd.setCursor(0,0);
              lcd.print("T1:" + String(t1));
            }

            //---Gol time 2---
            if(btn_click(btn_t2))
            {
              t2++;
              lcd.setCursor(0,1);
              lcd.print("T2:" + String(t2));
            }

            //---Conversão do tempo---
            tempAt = millis()-tempInit;
            tm = tempAt/60000;
            ts = tempAt/1000;
            if(tm>0)ts -= tm*60;

            //---Exibição do placar---
            lcd.setCursor(8,1);      
            lcd.print(String(tm)+':'+String(ts));
          }
          break;
        //---Opção do Dominó + Alarme---
        case 1:
          t1=0;t2=0;
          
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("T1:" + String(t1));
          lcd.setCursor(0,1);
          lcd.print("T2:" + String(t2));
          
          delay(50);

          //---Placar Dominó---
          while(!btn_click(btn_ok))
          {
            //---Pontos time 1---
            if(btn_click(btn_t1))
            {
              t1 += 5; 
              lcd.setCursor(0,0);
              lcd.print("T1:" + String(t1));
            }

            //---Pontos time2---
            if(btn_click(btn_t2))
            {
              t2 += 5;
              lcd.setCursor(0,1);
              lcd.print("T2:" + String(t2));
            }

            //---Alarme---
            if(t1==200 || t2==200)
            {
              alarme();
              break;
            }
          }
          break;
      }
    }

    //---Proxima opção---
    if(btn_click(btn_next))
    {
      lcd.clear();
      lcd.setCursor(0,0);
      if(op<1)op++;
      else op=0;
      lcd.print(menu[op]);
    }
}

//**************************************
//---Click dos botões----
bool btn_click(int btn)
{
  if(!digitalRead(btn)){
    delay(50);
    return true;
  }
  else return false;
}

//---Alarme---
void alarme()
{
  for (int frequencia = 150; frequencia < 1800; frequencia += 1) 
  {
    tone(pin_Buzzer, frequencia, tempo); 
    delay(1);
  }
  for (int frequencia = 1800; frequencia > 150; frequencia -= 1) 
  {
    tone(pin_Buzzer, frequencia, tempo); 
    delay(1);
  }
}
