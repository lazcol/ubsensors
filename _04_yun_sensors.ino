#include "Bridge.h"  // libreria conexion YUN WEB
#include "HttpClient.h" // libreria cliente http
#include "ThingSpeak.h" // libreria thingspeak (se baja de thingspeak)
#include "YunClient.h"  // libreria en la p´qagina de la dragino YUN
#include <Wire.h>  // librería I2C
#include <BH1750.h>  // Librería sensor de intensidad luminosa
#include "DHT.h"          // Librería sensor de Humedad y Temperatura DHT & AM2302
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

// vamos a modificar algo para que nos aparezca que se modificó el archivo

#define DHTPIN 2 // pin de conexión del SDA del sensor
#define DHTTYPE DHT22       // DHT 22  (AM2302)

YunClient client; //genera el cliente para el servicio web.

//ThingSpeak: datos para conexión, escritura y lectura
String thingSpeakAPI = "api.thingspeak.com"; // de la pagina de thingspeak
String talkBackAPIKey = "EFWWA5DCID6FBYHO";  // de la pagina de thingspeak
String talkBackID = "18565";  // de la pagina de thingspeak, talkback ID
const int checkTalkBackInterval = 15 * 1000;    // Intervalo para checkear el TalkBack (segundos * 1000 = intervalo)
unsigned long myChannelNumber = 347713; // de la pagina de thingspeak, es el numero de canal
const char * myWriteAPIKey = "2ZR7MG3GLXA1GC0D"; // el código que te habilita la escritura del canal, sale de thinsgpeak

// Se agregan estas 4 variables para guardar las lecturas de los sensores
long tem; // variable donde se guarda el valor de la temperatura
long pre; // variable donde se guarda el valor de la presión
long hum; // variable donde se guarda el valor de la humedad
//uint16_t lux; // variable donde se guarda el valor de los lúmenes
long lux;

BH1750 lightMeter;
DHT dht(DHTPIN, DHTTYPE);   // genera un objeto DHT que funciona con ese pin y es de ese tipo
Adafruit_BMP280 bme;

long lastConnectionTime = 0; // Variable para el control de la conexión (va por librería HTTPClient o YunClient)

void setup()
{
  // parpadeo del led en el pin 13 para ver cuando pasa por este punto
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);

  // Se inicializa el puente (bridge) a la página de ThingSpeak
  Bridge.begin(); // se inicializa sólo el puente
  ThingSpeak.begin(client); // se linkea el puente con la página de ThingSpeak y la YUN
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  lightMeter.begin(0x23);
  dht.begin();          // se pone en funcionamiento el sensor
  bme.begin(0x76);
}

// COMIENZO DEL PROGRAMA QUE VA A ESTAR LEYENDO LOS SENSORES Y ESCRIBIENDO LA PAGINA THINGSPEAK
void loop()
{   
  // Acá ponemos las lecturas de los sensores
  //do{
  tem = dht.readTemperature();
  hum = dht.readHumidity();
  //}while(isnan(t) || isnan(h));
  lux = lightMeter.readLightLevel();
  pre = bme.readPressure()/100;

  ThingSpeak.setField(1,tem);
  ThingSpeak.setField(2,hum);
  ThingSpeak.setField(3,pre);
  ThingSpeak.setField(4,lux);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  // Checkea a ThingSpeak para ver los comandos del TalkBack
  checkTalkBack(); // llamado de la función checkTalkBack que se encuentra al final del bloque
  delay(1000);
  checkTalkBack(); // llamado de la función checkTalkBack que se encuentra al final del bloque
  delay(1000);
  checkTalkBack(); // llamado de la función checkTalkBack que se encuentra al final del bloque
  delay(1000);
  checkTalkBack(); // llamado de la función checkTalkBack que se encuentra al final del bloque
  delay(checkTalkBackInterval);
}
void checkTalkBack() // En el entorno del IDE de Arduino, las funciones de definen al finalizar el bloque de programa y no arriba como en C
{
  HttpClient client; // genera un nuevo cliente HTTP ya que el otro se destruyó con la escritura
  
  String talkBackCommand;
  char charIn;
  String talkBackURL =  "http://" + thingSpeakAPI + "/talkbacks/" + talkBackID + "/commands/execute?api_key=" + talkBackAPIKey;
  
  // Realiza una solicitud tipo HTTP GET a la TalkBack API:
  client.get(talkBackURL);
    
  while (client.available()) { //Con esta función extrae letra por letra el comando
    charIn = client.read();
    talkBackCommand += charIn; 
  }
  //Serial.println(talkBackCommand); //esta era una línea para que mostrara por consola cual era el comando que recibía 
  // Enciende o apaga el led según la configuración en la página de ThingSpeak
  if (talkBackCommand == "Talta") //verificación de la temperatura
  {  
    digitalWrite(2, HIGH); //si temperatura es más alta que la asignada, enciende el led en el pin 2
  }
  else if (talkBackCommand == "Tbaja")
  {      
    digitalWrite(2, LOW); //si la temperatura es más baja que la asignada, apaga el led en el pin 2
  }
  if (talkBackCommand == "Palta") //verificación de la 
  {  
    digitalWrite(3, HIGH); //si la presión es más alta que la asignada, enciende el led en el pin 3
  }
  else if (talkBackCommand == "Pbaja")
  {      
    digitalWrite(3, LOW); //si la presión es más baja que la asignada, apaga el led en el pin 3
  }
  if (talkBackCommand == "Halta")
  {  
    digitalWrite(4, HIGH); //si la humedad es más alta que la asignada, enciende el led en el pin 4
  }
  else if (talkBackCommand == "Hbaja")
  {      
    digitalWrite(4, LOW); //si la humedad es más baja que la asignada, apaga el led en el pin 4
  }
  if (talkBackCommand == "Lalta")
  {  
    digitalWrite(5, HIGH); //si los lúmenes son más altos que los asignados, enciende el led en el pin 5
  }
  else if (talkBackCommand == "Lbaja")
  {      
    digitalWrite(5, LOW); //si los lúmenes son más bajos que los asignados, apaga el led en el pin 5
  }

  Serial.flush(); 
  delay(1000);
}
