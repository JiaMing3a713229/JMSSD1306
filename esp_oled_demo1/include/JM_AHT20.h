#ifndef _JM_SHT20_H
#include <stdint.h>
#define _JM_SHT20_H

#define TRIGGER_CMD 0XAC
#define TRIGGER_DATA1 0X33
#define TRIGGER_DATA2 0X00

struct i2c_dev{
    uint8_t slave_addr;

    float temperature;
    float humidity;
};

struct AHT20{

    struct i2c_dev *dev;

    float (*get_temperature)(struct AHT20 *self);
    float (*get_humidity)(struct AHT20 *self);

};

int init_aht20(struct AHT20 *self, uint8_t dev_slave_addr);




#endif //_JM_SHT20_H