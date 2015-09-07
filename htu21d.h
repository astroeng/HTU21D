

#ifndef HTU21D_H
#define HTU21D_H

#include <Software_I2C.h>

#define SAMPLE_TIME 55 /* ms */
#define HOLD_TIME 10 /* ms */

typedef enum
{
  HTU21D_RequestTemperature,
  HTU21D_WaitTemperature,
  HTU21D_RetrieveTemperature,
  HTU21D_RequestHumidity,
  HTU21D_WaitHumidity,
  HTU21D_RetrieveHumidity
} HTU21DReadStateType;

class HTU21D
{
public:
  HTU21D(Software_I2C* i2c_bus);
  void begin();
  HTU21DReadStateType run();
  
  int getTemperature();
  int getTemperatureAge();
  int getHumidity();
  int getHumidityAge();
private:
  unsigned int rawTemperature;
  unsigned int rawHumidity;
  
  unsigned long temperatureTime;
  unsigned long humidityTime;
  
  unsigned long requestTime;

  char readState;
  Software_I2C* _i2c_bus;

};




#endif