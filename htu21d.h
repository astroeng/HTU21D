

#ifndef HTU21D_H
#define HTU21D_H

#include <Software_I2C.h>

#define SAMPLE_TIME 55 /* ms */
#define HOLD_TIME 10 /* ms */

typedef enum
{
  RequestTemperature,
  WaitTemperature,
  RetrieveTemperature,
  RequestHumidity,
  WaitHumidity,
  RetrieveHumidity
} HTU21DReadStateType;

class HTU21D
{
public:
  HTU21D(Software_I2C* i2c_bus);
  void readData();
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