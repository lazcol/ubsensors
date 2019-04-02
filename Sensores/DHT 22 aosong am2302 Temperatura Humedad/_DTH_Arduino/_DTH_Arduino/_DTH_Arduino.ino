#include "DHT.h"          // DHT & AM2302 library

#define DHTPIN 2 // pin de conexión del SDA del sensor
#define DHTTYPE DHT22       // DHT 22  (AM2302)
float h;
float t;

DHT dht(DHTPIN, DHTTYPE);   // genera un objeto DHT que funciona con ese pin y es de ese tipo

void setup() {
  Serial.begin(9600);   // Setup monitor serial USB
  dht.begin();          // se pone en funcionamiento el sensor
}

void loop() {
  // leer la temperatura y la humedad toma alrrededor de 250 milisegundos
  // Freq de muestreo 2Hz
  do{
    h = dht.readHumidity();
    t = dht.readTemperature();
  }while(isnan(t) || isnan(h));
 
    Serial.print(F("Humedad: ")); 
    Serial.print(h);
    Serial.print(F(" %\t"));
    Serial.print(F("Temperatura: ")); 
    Serial.print(t);
    Serial.println(F(" C"));
 
  
  // Wait 3 seconds
  delay(3000);
}
