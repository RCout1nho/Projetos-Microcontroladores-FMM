# Execução
Para diminuir o número de porta utilizadas pelo display LCD, foi utilizado o módulo I2C para LCD, que diminui os 16 pinos do LCD para apenas 4. Para utilizar o módulo I2C é necessária a biblioteca “LiquidCrystal_I2C”¸a mesma pode ser encontrada nesse mesmo repositório.

## Código
```
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
long int tempInit,tempAt,ts,tm,op=0,t1,t2;
```
1.```String menu[2]= { "Futsal", "Domino"}``` é utilizado para conter as opções existentes, visto que um array contém posição numérica e 
respectivo valor, o que o torna ideal para navegação em menu.

2.```int op=0```indica qual a opção selecionada dentro do menu.(op=0 => Futsal, op=1 => Domino).

3.```int tempInit``` e ```int temAt``` servem para armazenar respectivamente em quanto tempo após a inicialização do Arduino a opção que 
mostra o cronômetro foi selecionada e em quanto tempo está o processo atual, de forma que tempAt-tempInit = variação do tempo.

4.```int tm``` e ```int ts``` servem para armazenar o tempo em minutos e segundos, respectivamente.As variáveis que armazenam o tempo 
precisam ser do tipo long int pois após certo tempo, os valores em milissegundos ficaram muito grandes.

```
//**************************************
void setup()
{
  pinMode(btn_ok,INPUT_PULLUP);
  pinMode(btn_t1,INPUT_PULLUP);
  pinMode(btn_t2,INPUT_PULLUP);
  pinMode(btn_next,INPUT_PULLUP);

  lcd.init(); //Inicia o LCD
  lcd.backlight(); // Ativa a luz de fundo do LCD
  lcd.print("Micro- AV3");
}
```
1. Os botões foram declarados como INPUT_PULLUP para que facilite o uso de hardware, descartando o uso de resistor em séries com o 
botão.

```
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
```
1. Função foi criada com o intuito de facilitar a leitura do estado dos botões.
2. O delay(50) é necessário pois, caso não exista, o Arduino faz a leitura de múltiplos toques no botão, visto que cada leitura ocorrerá 
em tempo de processamento, que é muito rápido, o que torna o delay uma forma de idenficiar toques em tempos diferentes no botão.

```
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
```
1. A função consiste em uma rotina que faz com que o buzzer faça o som de uma sirene de polícia.

```
void loop()
{
    if(btn_click(btn_ok))// Se  o botão Ok for apertado
    {
      switch(op)
      {
        //---Opção Futsal---
        case 0:
          tempInit = millis();//Tempo inicial
          
          t1=0; t2=0;
          
          lcd.clear();          
          lcd.setCursor(0,0);
          lcd.print("T1:" + String(t1));
          lcd.setCursor(0,1);
          lcd.print("T2:" + String(t2));
          delay(50);
```
Caso a opção “Futsal” seja escolhida:
1.  O display é limpado, reposicionado na posição (0,0), e exibe-se “T1: “ indicando a pontuação do time 1 e “T2: “ indicando a pontuação do time 2.

```
          while(!btn_click(btn_ok))// Enquando o botão OK não é apertado
          {
            //----Gol time 1---
            if(btn_click(btn_t1))// Se o botão T1 for apertado
            {
            t1++; 
            lcd.setCursor(0,0);
            lcd.print("T1:" + String(t1));
            }

            //---Gol time 2---
            if(btn_click(btn_t2))// Se o botão T2 for apertado
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
          break
```
1. O tempo contado pelo Arduino através na função millis() representa o tempo em milissegundos a partir do ligamento do mesmo, 
necessário calcular esse tempo apenas após iniciar uma partida.
2. Para calcular o tempo a partir do momento em que a partida é iniciada é calculada a diferença entre o tempo atual e o tempo em que a 
partida foi iniciada, tendo assim o tempo passado em uma partida.
3. Para converter esse tempo em milissegundos para minutos e segundos, é feita o seguinte cálculo:
-> Para obter o tempo em segundos basta dividi-lo por 1000, uma vez que milissegundo = (1/1000)segundos.
-> Para obter o tempo em minutos basta dividir o tempo em segundos por 60, ou o tempo em milissegundos por 60000, utilizando apenas a 
parte inteira da divisão e depois subtraindo 60*(quantidade de minutos) para obter os segundos restantes. 
### Por exemplo:
150*10-3 segundos (150 milissegundos)
1) (150*10-3)/(103) 150 segundos.
2) 150/60 2 minutos (Divisão inteira).
3) 150 – 2*60  30 segundos.
RESULTADO = 2 minutos e 30 segundos.

```
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
```
1. Exatamente a mesma rotina que ocorre no jogo de futsal.
2. Possui um alarme que é tocado quando uma das equipes atinge 200 pontos.
3. O botão OK serve para retornar ao menu anterior.

```
    //---Proxima opção---
    if(btn_click(btn_next))
    {
      lcd.clear();
      lcd.setCursor(0,0);
      if(op<1)op++;
      else op=0;
      lcd.print(menu[op]);
    }
```
Ao ser apertado faz o display alternar entre as opções.
