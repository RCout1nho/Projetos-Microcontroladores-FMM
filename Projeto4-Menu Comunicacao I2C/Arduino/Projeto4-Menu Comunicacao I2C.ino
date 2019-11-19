//---Bibliotecas---
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Wire.h>

//---Pinos---
#define ldr_pin A0
#define relay_pin 5
#define led_pin 6
#define btnOk 7
#define btnNext 8
#define dht_pin 9

//----Endereços---
#define my_Adress 0x08
#define slave_Adress 0x09

//---Objetos---
dht DHT;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//***************************************************************************************
//---Opcões do Menu---
String mainMenu[4] = {"Sensor Local", "Sensor Remoto", "Atuador Local", "Atuador Remoto"};
String subMenu1[3] = {"Temperatura", "Luminosidade", "Umidade"};
String subMenu2[3] = {"AC", "DC", "Sair"};
String aux[3];

//---Variaveis auxiliares---
bool subMenu = false, menuSens = false, local = false, remoto = false, menuAtuador = false;
int mainOp = -1, subOp = 0, contAux = 0, l = 0;
String tp, lm, um;
int z = 0;

//***************************************************************************************
void setup() {
  Wire.begin(my_Adress);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  Serial.begin(9600);

  pinMode(btnOk, INPUT_PULLUP);
  pinMode(btnNext, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);

  digitalWrite(relay_pin, HIGH);

  lcd.init();
  lcd.backlight();
  lcd.print("Micro- Parcial 2");
}

//***************************************************************************************
void loop() {
  Debug();
  Debug_Dados();

  //---Menu principal(ok)---
  if (ok_Click() && !subMenu) {
    subMenu = true;
    switch (mainOp) {
      case 0:
        menuSens = true;
        local = true;
        goto Sensor_Local;
        break;

      case 1:
        menuSens = true;
        local = false;
        remoto = true;
        goto Sensor_Remoto;
        break;

      case 2:
        menuAtuador = true;
        local = true;
        remoto = false;
        goto Atuador;

      case 3:
        menuAtuador = true;
        local = false;
        remoto = true;
        goto Atuador;
        break;
    }
  }

  //---Menu sensores(ok)---
  if (ok_Click() && subMenu && menuSens) {
    subMenu = false;
    menuSens = false;
    local = false;
    subOp = 0;
    mainOp = -1;
    goto Main_Menu;
  }

  //---Menu atuador local(ok)---
  if (ok_Click() && subMenu && menuAtuador && local) {
    Serial.println("Atuador Local");
    if (subOp == 1 && contAux > 0) {
      Serial.println("Led");
      Carga_Dc();
    }

    if (subOp == 0 && contAux > 0) {
      Serial.println("Relay");
      Carga_Ac();
    }

    if (subOp == 2 && contAux > 0) {
      subMenu = false;
      local = false;
      remoto = false;
      menuAtuador = false;
      subOp = 0;
      goto Main_Menu;
    }
    contAux++;
  }

  //---Menu atuador remoto---
  if (ok_Click() && subMenu && menuAtuador && remoto) {
    Serial.println("Atuador Remoto");
    if (subOp == 1 && contAux > 0) {
      Serial.println("Led");
      Carga_Dc_Rem();
    }

    if (subOp == 0 && contAux > 0) {
      Serial.println("Relay");
      Carga_Ac_Rem();
    }

    if (subOp == 2 && contAux > 0) {
      subMenu = false;
      local = false;
      remoto = false;
      menuAtuador = false;
      subOp = 0;
      goto Main_Menu;
    }
    contAux++;
  }
  //---Menu principal(next)---
  if (next_Click() && !subMenu) {
Main_Menu:
    lcd.clear();
    lcd.setCursor(0, 0);
    if (mainOp < 3)mainOp++;
    else mainOp = 0;
    lcd.print(mainMenu[mainOp]);
  }

  //---Menu Sensor local(next)---
  if (next_Click() && subMenu && menuSens && local) {
Sensor_Local:
    lcd.clear();
    lcd.setCursor(0, 0);
    if (subOp < 2) subOp++;
    else subOp = 0;
    lcd.print(subMenu1[subOp]);
    lcd.setCursor(0, 1);
    if (subOp == 0) lcd.print(Med_Temp());
    if (subOp == 1) lcd.print(Med_Lum());
    if (subOp == 2) lcd.print(Med_Umd());
  }

  //---Menu Sensor Remoto(next)---
  if (next_Click() && subMenu && menuSens && remoto) {
Sensor_Remoto:
    Wire.requestFrom(slave_Adress, 12);
    String Rx1 = Wire.readStringUntil('\n');
    Serial.println("Wire.read() = " + Rx1);
    aux[0] = ""; aux[1] = ""; aux[2] = "";
    l = 0;
    for (int i = 0; i < Rx1.length() - 1; i++) {
      if (Rx1[i] != '-') {
        aux[l] += Rx1[i];
      } else {
        l++;
      }
    }

    Serial.println(aux[0] + ',' + aux[1] + ',' + aux[2]);
    lcd.clear();
    lcd.setCursor(0, 0);
    if (subOp < 2) subOp++;
    else subOp = 0;
    lcd.print(subMenu1[subOp]);
    lcd.setCursor(0, 1);
    if (subOp == 0) {
      lcd.setCursor(0, 1);
      lcd.print(aux[0]);
    }
    if (subOp == 1) {
      lcd.setCursor(0, 1);
      lcd.print(aux[1]);
    }
    if (subOp == 2) {
      lcd.setCursor(0, 1);
      lcd.print(aux[2]);
    }
  }

  //---Menu atuadores(next)---
  if (next_Click() && subMenu && menuAtuador) {
Atuador:
    lcd.clear();
    lcd.setCursor(0, 0);
    if (subOp < 2) subOp++;
    else subOp = 0;
    lcd.print(subMenu2[subOp]);
  }
}

//***************************************************************************************
//---Click dos butões---
bool ok_Click() {
  if (!digitalRead(btnOk)) {
    Serial.println("OK");
    delay(100);
    return true;
  } else {
    return false;
  }
}

bool next_Click() {
  if (!digitalRead(btnNext)) {
    Serial.println("Next");
    delay(100);
    return true;
  } else {
    return false;
  }
}

//***************************************************************************************
//---Medicão de sensores---
String Med_Temp()
{
  DHT.read11(dht_pin);
  int temp = DHT.temperature;
  return String(temp);
}

String Med_Umd() {
  DHT.read11(dht_pin);
  int umd = DHT.humidity;
  return String(umd);
}

String Med_Lum()
{
  int lum = analogRead(ldr_pin);
  return String(lum);
}

//***************************************************************************************
//---Ativação de cargas---
void Carga_Dc() {
  digitalWrite(led_pin, !digitalRead(led_pin)); \
}

void Carga_Ac() {
  digitalWrite(relay_pin, !digitalRead(relay_pin));
}

void Carga_Dc_Rem() {
  Wire.beginTransmission(slave_Adress);
  Wire.println("L");
  Wire.endTransmission();
}

void Carga_Ac_Rem() {
  Wire.beginTransmission(slave_Adress);
  Wire.println("R");
  Wire.endTransmission();
}

//***************************************************************************************
//---Comunicação---
void requestEvent() {
  Serial.println("Entrei");
  Serial.println(tp + lm + um);
  Wire.println(tp + '-' + lm + '-' + um);
}

void receiveEvent() {
  String aux = Wire.readStringUntil('\n');
  Serial.println(aux);
  if (aux[0] == 'L') {
    Serial.println("led");
    digitalWrite(led_pin, !digitalRead(led_pin));
  }

  if (aux[0] == 'R') {
    Serial.println("rele");
    digitalWrite(relay_pin, !digitalRead(relay_pin));
  }
}

//***************************************************************************************
//---Funções auxiliares---
void Debug_Dados() {
  tp = Med_Temp();
  lm = Med_Lum();
  um = Med_Umd();
  Wire.flush();
  if (Wire.available() > 0) {
    String Rx1 = Wire.readStringUntil('\n');
    Serial.println(Rx1);

    for (int i = 0; i < Rx1.length(); i++) {
      if (Rx1[i] != '-') {
        aux[l] += Rx1[i];
      } else {
        z++;
      }
    }
    int i = 0;
    Serial.println("Temp=" + aux[i++]);
    Serial.println("Lum=" + aux[i++]);
    Serial.println("Umd=" + aux[i++]);
  }
}

void Debug() {
  if (Serial.available() > 0) {
    if (Serial.read() == '.') {
      Serial.println(subMenu);
      Serial.println(menuSens);
      Serial.println(local);
      Serial.println(remoto);
      Serial.println(menuAtuador);
      Serial.println(mainOp);
      Serial.println(subOp);
      Serial.println("****************");
    }
  }
}