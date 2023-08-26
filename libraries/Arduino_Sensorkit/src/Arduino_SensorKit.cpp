#include "Arduino_SensorKit.h"

//Declare component's classes
U8X8_SSD1306_128X64_NONAME_SW_I2C Oled(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
DHT Environment(DHTPIN,DHT11);
DHT Environment_I2C(DHT20);
SensorKit_LIS3DHTR Accelerometer;
SensorKit_BMP280 Pressure;