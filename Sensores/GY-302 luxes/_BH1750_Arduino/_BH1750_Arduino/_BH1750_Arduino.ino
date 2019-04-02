/*
Connection:

    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND

  ADD pin => 0.7VCC => sensor address 0x5C
  ADD pin <  0.7VCC => sensor address 0x23
*/

#include <Wire.h>
#include <BH1750.h>

/*
  Library uses 0x23 address as default, but you can define the other address.
  If you had troubles with default value - try to change it to 0x5C.
*/
BH1750 lightMeter(0x23);

void setup(){

  Serial.begin(9600); /*no se usa, porque no se utiliza consola*/

  // Inicializa la comunicación I2C. La librería no lo hace automáticamente
  Wire.begin();
//lightMeter.begin();
lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2);
  /*

    BH1750 has six different measurement modes. They are divided in two groups;
    continuous and one-time measurements. In continuous mode, sensor continuously
    measures lightness value. In one-time mode the sensor makes only one
    measurement and then goes into Power Down mode.

    Each mode, has three different precisions:

      - Low Resolution Mode - (4 lx precision, 16ms measurement time)
      - High Resolution Mode - (1 lx precision, 120ms measurement time)
      - High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)

    By default, the library uses Continuous High Resolution Mode, but you can
    set any other mode, by passing it to BH1750.begin() or BH1750.configure()
    functions.

    [!] Remember, if you use One-Time mode, your sensor will go to Power Down
    mode each time, when it completes a measurement and you've read it.

    Full mode list:

      BH1750_CONTINUOUS_LOW_RES_MODE
      BH1750_CONTINUOUS_HIGH_RES_MODE (default)
      BH1750_CONTINUOUS_HIGH_RES_MODE_2

      BH1750_ONE_TIME_LOW_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE
      BH1750_ONE_TIME_HIGH_RES_MODE_2

  */

  /* NO SE PONE YA QUE NO SE TIENE FORMA DE AVERIGUAR SI HAY PROBLEMA CON EL SENSOR
  // retorna un booleano que puede servir para ver si el sensor se inició bien
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 Advanced begin"));
  }
  else {
    Serial.println(F("Error initialising BH1750"));
  } 
*/
}


void loop() {

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

}
