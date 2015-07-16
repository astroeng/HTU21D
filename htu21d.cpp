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
  rawTemperature = 0;
  rawHumidity = 0;
  temperatureTime = 0;
  humidityTime = 0;
  requestTime = 0;
  readState = RequestTemperature;

}


void HTU21D::readData()
{
  char error;
  unsigned char data[3];

  
  /* This is the switch block that implements the functional activities for each
     of the states.
   */

  switch (readState)
  {
    /* In the request temperature state the command to start the temperature conversion
       is sent to the device. The time that the request was sent is saved for use in the
       state flow logic in the next switch block.
     */
    case RequestTemperature:
      _i2c_bus->start_i2c();
      _i2c_bus->write(HTU21D_ADDRESS);
      _i2c_bus->write(START_TEMPERATURE_NOHOLD);
      _i2c_bus->stop_i2c();
      
      requestTime = millis();
      
      break;

    /* In the retrieve temperature state the i2c bus is used to read back the value
       that was triggered in the previous state. This value consists of 3 bytes (high,
       low, crc). For the moment the crc is ignored.
       TODO: Implement crc functionality.
     */
    case RetrieveTemperature:
      _i2c_bus->start_i2c();
      _i2c_bus->write(HTU21D_ADDRESS | 0x01);
      
      temperatureTime = millis();
      
      data[0] = _i2c_bus->read(&error);
      data[1] = _i2c_bus->read(&error);
      data[2] = _i2c_bus->read(&error);
      _i2c_bus->stop_i2c();
      
      rawTemperature = ((int)data[0] << 8) | data[1];
      
      break;
      
    /* The request humidity state is pretty much the same as the request temperature 
       state excepth the command for humidity is sent.
     */
    case RequestHumidity:

      _i2c_bus->start_i2c();
      _i2c_bus->write(HTU21D_ADDRESS);
      _i2c_bus->write(START_HUMIDITY_NOHOLD);
      _i2c_bus->stop_i2c();
      
      requestTime = millis();
      
      break;
    
    /* The retrieve humidity state is pretty much the same as the retrieve temperature
       state except the bytes from the humidity command are returned.
       TODO: Implement CRC functions.
     */
    case RetrieveHumidity:
      
      _i2c_bus->start_i2c();
      _i2c_bus->write(HTU21D_ADDRESS | 0x01);
      
      humidityTime = millis();
      
      data[0] = _i2c_bus->read(&error);
      data[1] = _i2c_bus->read(&error);
      data[2] = _i2c_bus->read(&error);
      _i2c_bus->stop_i2c();
      
      rawHumidity = ((int)data[0] << 8) | data[1];
      
      break;
    default:
      /* do nothing */
      /* WaitTemperature */
      /* WaitHumidity */
      break;
  }
  
  /* This is the state flow logic. It must be below the state activity logic.
   */
  
  switch (readState) 
  {
    case RequestTemperature:
      readState = WaitTemperature;
      break;
    case WaitTemperature:
      if (requestTime + SAMPLE_TIME < millis())
      {
        readState = RetrieveTemperature;
      }
      break;
    case RetrieveTemperature:
      readState = RequestHumidity;
      break;
    case RequestHumidity:
      readState = WaitHumidity;
    case WaitHumidity:
      if (requestTime + SAMPLE_TIME < millis())
      {
        readState = RetrieveHumidity;
      }
      break;
    case RetrieveHumidity:
      readState = RequestTemperature;
      break;
  }

}

int HTU21D::getTemperature()
{
  double value = 0;
  
  value = -46.85 + 175.72 * rawTemperature / 65536.0;
  
  return value * 10.0;

}

int HTU21D::getTemperatureAge()
{
  
  return millis() - temperatureTime;
  
}


int HTU21D::getHumidity()
{
  double value = 0;
  
  value = -6.0 + 125.0 * rawHumidity / 65536.0;
  
  return value * 10.0;
  
}  

int HTU21D::getHumidityAge()
{
  
  return millis() - humidityTime;
  
}  