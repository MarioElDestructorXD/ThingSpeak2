#include "ThingSpeak.h"
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

OneWire ourWire(2);
DallasTemperature sensors(&ourWire);
#define DHTPIN 4
#define DHTTYPE DHT11

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;


DHT dht(DHTPIN, DHTTYPE);
String data ;
int SensorPin = A0;
int cont = 0;
float te;
float hum;
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  sensors.begin(); //Se inicia el sensor
  ThingSpeak.begin(client);
  dht.begin();
  pinMode(13, OUTPUT);
}

long tiempoUltimaLectura = 0;

void loop() {

  //variables
  int humedad = analogRead(SensorPin);
  float humedadTot = 100 - (humedad / 10.23);

  delay(1000);
  //.........................................SENSOR DS18B20 DE TEMPERATURA
  sensors.requestTemperatures(); //Se envía el comando para leer la temperatura
  float temp = sensors.getTempCByIndex(0);

  delay(500);

  if (millis() - tiempoUltimaLectura > 2000)
  {

    float h = dht.readHumidity(); //Leemos la Humedad
    float t = dht.readTemperature(); //Leemos la temperatura en grados Celsius
    float f = dht.readTemperature(true); //Leemos la temperatura en grados Fahrenheit

    hum = h;
    te = t;
    Serial.print(temp);
    Serial.print(" , ");
    Serial.print(te);
    Serial.print(" , ");
    Serial.print(hum);
    Serial.print(" , ");
    Serial.println(humedadTot);

    tiempoUltimaLectura = millis(); //actualizamos el tiempo de la última lectura


  }


  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID:UTEZ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Measure Signal Strength (RSSI) of Wi-Fi connection
  long rssi = WiFi.RSSI();





  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, temp, myWriteAPIKey);
  delay(2000);
  int httpCode2 = ThingSpeak.writeField(myChannelNumber, 2, te, myWriteAPIKey);
  delay(2000);
  int httpCode3 = ThingSpeak.writeField(myChannelNumber, 3, hum, myWriteAPIKey);
  delay(2000);
  int httpCode4 = ThingSpeak.writeField(myChannelNumber, 4, humedadTot, myWriteAPIKey);
  delay(2000);
  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
  if (httpCode2 == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  if (httpCode3 == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  if (httpCode4 == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  tiempoUltimaLectura = millis();
  // Wait 20 seconds to update the channel again
  delay(2000);
}
