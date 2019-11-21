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
1. ```String menu[2]= { "Futsal", "Domino"}``` é utilizado para conter as opções existentes, visto que um array contém posição numérica e respectivo valor, o que o torna ideal para navegação em menu.
2.```int op=0```indica qual a opção selecionada dentro do menu.(op=0 => Futsal, op=1 => Domino).
3.```int tempInit``` e ```int temAt``` servem para armazenar respectivamente em quanto tempo após a inicialização do Arduino a opção que mostra o cronômetro foi selecionada e em quanto tempo está o processo atual, de forma que tempAt-tempInit = variação do tempo.
4.int tm e int ts servem para armazenar o tempo em minutos e segundos, respectivamente.
4. As variáveis que armazenam o tempo precisam ser do tipo long int pois após certo tempo, os valores em milissegundos ficaram muito grandes.


