

#ifndef HTU21D_H
#define HTU21D_H

#include <Software_I2C.h>

class HTU21D
{
public:
  HTU21D(Software_I2C* i2c_bus);
  void readData();
  int getTemperature();
  int getHumidity();
private:
  unsigned int rawTemperature;
  unsigned int rawHumidity;
  Software_I2C* _i2c_bus;

};




#endif