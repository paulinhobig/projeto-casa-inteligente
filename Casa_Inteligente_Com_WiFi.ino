/*
   Código Exemplo: 
*/
#include <Servo.h>
Servo servoD4;
Servo servoD5;
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Lowran" // Coloque o nome do seu WiFi aqui
#define STAPSK "12345678" // Coloque a senha do seu WiFi aqui
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
#define SALA 16
#define COZINHA 5
#define QUARTO 4
#define BANHEIRO 0

// Criar a instancia do servidor
// Especificar a porta que vai ser utilizada
WiFiServer server(80);

void setup() {
  servoD4.attach(2);
  servoD5.attach(14);
  Serial.begin(9600);

  // definindo o led
  pinMode(SALA, OUTPUT);
  digitalWrite(SALA, 0);
  pinMode(COZINHA, OUTPUT);
  digitalWrite(COZINHA, 0);
  pinMode(BANHEIRO, OUTPUT);
  digitalWrite(BANHEIRO, 0);

  pinMode(QUARTO, OUTPUT);
  digitalWrite(QUARTO, 0);

  // COnectando a rede WiFi
  Serial.println();
  Serial.println();
  Serial.print(F("Conectando do WiFi: "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi conectado!"));

  // Iniciar o servidor
  server.begin();
  Serial.println(F("Servidor iniciado..."));

  // Informar o endereço IP
  Serial.println(WiFi.localIP());
}

void loop() {
  //Checar se um cliente está conectado
  WiFiClient client = server.accept();
  if (!client) { return; }
  Serial.println(F("Nova solicitação!"));

  client.setTimeout(5000);  

  // recebendo a linha de solciitação
  String req = client.readStringUntil('\r');
  Serial.println(F("Solicitação: "));
  Serial.println(req);

  // validando a solicitação
  int val;
  if (req.indexOf(F("/SALA_ON")) != -1) {
    digitalWrite(SALA, HIGH);

  } else if (req.indexOf(F("/SALA_OFF")) != -1) {
    digitalWrite(SALA, LOW);

  } else if (req.indexOf(F("/COZINHA_ON")) != -1) {
    digitalWrite(COZINHA, HIGH);

  } else if (req.indexOf(F("/COZINHA_OFF")) != -1) {
    digitalWrite(COZINHA, LOW);

  } else if (req.indexOf(F("/BANHEIRO_ON")) != -1) {
    digitalWrite(BANHEIRO, HIGH);

  } else if (req.indexOf(F("BANHEIRO_OFF")) != -1) {
    digitalWrite(BANHEIRO, LOW);

  } else if (req.indexOf(F("PORTAO_ABRIR")) != -1) {
    servoD4.write(0);
    delay(100);
    servoD4.write(180);
    delay(100);
  }
  else if (req.indexOf(F("PORTAO_FECHAR")) != -1) {
    servoD4.write(180);
    delay(100);
    servoD4.write(0);
    delay(100);
  }
  else if (req.indexOf(F("QUARTO_ON")) != -1) {
    digitalWrite(QUARTO, HIGH);
  }
  else if (req.indexOf(F("QUARTO_OFF")) != -1) {
    digitalWrite(QUARTO, LOW);
  }

  else if (req.indexOf(F("PORTA_ABRIR_")) != -1) {
    servoD5.write(0);
    delay(100);
    servoD5.write(180);
    delay(100);
  }
  else if (req.indexOf(F("PORTA_FECHAR_")) != -1) {
    servoD5.write(180);
    delay(100);
    servoD5.write(0);
    delay(100);
  }

  // Configurando o led de acordo com a solicitação


 
  while (client.available()) {
    client.read();
  }

  // Corpo do site
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nO LED está  "));
  client.print((val) ? F("Desligado") : F("Ligado"));
  client.print(F("<br><br>Clique <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/on'>here</a> para ligar o LED, ou <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/off'>Aqui</a> para desligar o LED.</html>"));
 
 
}