// Define portas e bibliotecas do sensor DHT11 (temperatura)
#include "DHT.h"
#define DHTPIN 2     // Pino digital sensor DHT
# define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Definir porta sensor de umidade
#define umidadeSensor A0

//Display
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define endereco  0x27 // Endereços comuns: 0x27, 0x3F
#define colunas   16
#define linhas    2

LiquidCrystal_I2C lcd(endereco, colunas, linhas);

int ledsVermelho[] = {12,4};
int ledsVerde[] = {10,8};

#define bombaAgua 13

int contador = 0;
int comutador = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx teste!"));
    dht.begin();

  pinMode(umidadeSensor, INPUT);
  pinMode(bombaAgua, OUTPUT); 
  digitalWrite(bombaAgua, LOW);
  for(int i=0; i<2; i++) {
    pinMode(ledsVermelho[i], OUTPUT);
    pinMode(ledsVerde[i], OUTPUT);
  } 
  lcd.init(); // INICIA A COMUNICAÇÃO COM O DISPLAY
  lcd.backlight(); // LIGA A ILUMINAÇÃO DO DISPLAY  
  lcd.clear(); // LIMPA O DISPLAY
  lcd.setCursor(0,0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0,1);
  lcd.print("System...");
  delay(4000);
  lcd.clear(); // LIMPA O DISPLAY
  lcd.setCursor(0,0);
  lcd.print("Regulando os");
  lcd.setCursor(0,1);
  lcd.print("sensores");
  delay(3000);
  digitalWrite(ledsVermelho[0], HIGH);
  delay(1000);
  digitalWrite(ledsVermelho[0], LOW);
  digitalWrite(ledsVermelho[1], HIGH);
  delay(1000);
  digitalWrite(ledsVermelho[1], LOW);
  digitalWrite(ledsVerde[0], HIGH);
  delay(1000);
  digitalWrite(ledsVerde[0], LOW);
  digitalWrite(ledsVerde[1], HIGH);
  delay(1000);
  digitalWrite(ledsVerde[1], LOW);
  delay(500);
}


void loop() {
  
  float h = dht.readHumidity();
  // Temperature em Celsius (default)
  float t = dht.readTemperature();
  float u = (map((analogRead(umidadeSensor)), 1023,0,0,10000));
  float  umidadeLeitura = (u / 100);

  // Verifique se alguma leitura falhou e tenta novamente.
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Falha de leitura do sensor DHT!"));
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));


  if(umidadeLeitura<55) {
      delay(500);
      comutador = 0;
      if (contador == 0) {
        for(int i=0; i<2; i++) {
                digitalWrite(ledsVermelho[i], HIGH);
                digitalWrite(ledsVerde[i], LOW);  
                delay(100);   
                digitalWrite(ledsVermelho[i], LOW);
                digitalWrite(ledsVerde[i], LOW);    
                delay(100);      
                digitalWrite(ledsVermelho[i], HIGH);
                digitalWrite(ledsVerde[i], LOW);  
                delay(100);   
                digitalWrite(ledsVermelho[i], LOW);
                digitalWrite(ledsVerde[i], LOW);    
                delay(100);
  
        }
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Preciso de");
          lcd.setCursor(0,1);
          lcd.print("agua... :(");

          delay(3000);        
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Preparando para");
          lcd.setCursor(0,1);
          lcd.print("irrigar...");
          delay(10);
          contador = !contador;
          delay(2000);
      }


      for(int i=0; i<2; i++) {
        digitalWrite(ledsVermelho[i], HIGH);
        digitalWrite(ledsVerde[i], LOW);        
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Irrigando... :D");
      delay(10);
      lcd.setCursor(0,1);
      lcd.print("Umidade: ");
      lcd.print(umidadeLeitura);
      lcd.print("%");
      digitalWrite(bombaAgua, HIGH);
      delay(150);
  
  } else {

    digitalWrite(bombaAgua, LOW);
    for(int i=0; i<2; i++) {
      digitalWrite(ledsVermelho[i], LOW);
      digitalWrite(ledsVerde[i], HIGH);
    }
    delay(100);
    if(comutador == 0) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Ok,estou");
      lcd.setCursor(0,1);
      lcd.print("umida agora!!");
      comutador = !comutador;
      delay(4000);
    }
    contador = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Umidade: ");
    lcd.print(umidadeLeitura);
    lcd.print("%");
    lcd.setCursor(0,1); 
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.write(B11011111);
    lcd.print("C");
    delay(1000);
  }
}
