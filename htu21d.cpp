#include <software_i2c.h>
#include <htu21d.h>
#include <Arduino.h>

/* The address is the upper 7 bits of the address word. The lower bit is
 read or write.
 */

#define HTU21D_ADDRESS 0x80 // This is really 0x40 but this is preshifted.  

#define START_TEMPERATURE_AND_HOLD 0xE3
#define START_HUMIDITY_AND_HOLD    0xE5
#define START_TEMPERATURE_NOHOLD   0xF3
#define START_HUMIDITY_NOHOLD      0xF5
#define WRITE_USER_REG        0xE6
#define READ_USER_REG         0xE7
#define SOFT_RESET            0xFE


HTU21D::HTU21D(Software_I2C* i2c_bus)
{
  
  _i2c_bus = i2c_bus;

}


void HTU21D::readData()
{
  char error;
  
  unsigned char data[3];

  _i2c_bus->start_i2c();
  _i2c_bus->write(HTU21D_ADDRESS);
  _i2c_bus->write(START_TEMPERATURE_AND_HOLD);
  _i2c_bus->stop_i2c();
  delay(55);
  _i2c_bus->start_i2c();
  _i2c_bus->write(HTU21D_ADDRESS | 0x01);
  data[0] = _i2c_bus->read(&error);
  data[1] = _i2c_bus->read(&error);
  data[2] = _i2c_bus->read(&error);
  
  rawTemperature = ((int)data[0] << 8) | data[1];

  _i2c_bus->start_i2c();
  _i2c_bus->write(HTU21D_ADDRESS);
  _i2c_bus->write(START_HUMIDITY_AND_HOLD);
  _i2c_bus->stop_i2c();
  delay(55);
  _i2c_bus->start_i2c();
  _i2c_bus->write(HTU21D_ADDRESS | 0x01);
  data[0] = _i2c_bus->read(&error);
  data[1] = _i2c_bus->read(&error);
  data[2] = _i2c_bus->read(&error);

  
  rawHumidity = ((int)data[0] << 8) | data[1];

}

int HTU21D::getTemperature()
{
  double value = 0;
  
  value = -46.85 + 175.72 * rawTemperature / 65536.0;
  
  return value * 10.0;

}


int HTU21D::getHumidity()
{
  double value = 0;
  
  value = -6.0 + 125.0 * rawHumidity / 65536.0;
  
  return value * 10.0;
  
}