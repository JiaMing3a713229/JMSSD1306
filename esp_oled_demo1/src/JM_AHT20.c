#include "JM_AHT20.h"
#include <driver/i2c.h>

#define AHT20_CMD_INIT              0x71
#define AHT20_CMD_TRIGGER_MEASURE   0xAC
#define I2C_MASTER_NUM              0


static int _aht20_init_cmd_(struct AHT20 *self){

    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    int err = -1;

    // 發送0x71命令讀取狀態字節
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (self->dev->slave_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_handle, AHT20_CMD_INIT, true);
    i2c_master_stop(cmd_handle);
    err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd_handle, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd_handle);
    vTaskDelay(10 / portTICK_PERIOD_MS);

    return err;

}
/*
**@brief this function used to send command bytes 0xAC
** following 0x33 and 0x00
*/
static int _aht20_trigger_cmd_(struct AHT20 *self){

    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    int err = 0;
    

    uint8_t measure_cmd[2] = {0x33, 0x00};
    cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (self->dev->slave_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd_handle, AHT20_CMD_TRIGGER_MEASURE, true);
    i2c_master_write(cmd_handle, measure_cmd, sizeof(measure_cmd), true);
    i2c_master_stop(cmd_handle);
    err = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd_handle, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd_handle);
    vTaskDelay(80 / portTICK_PERIOD_MS);
    
    // if(err != ESP_OK){
    //     printf("aht20 trigger successed FAIL !\r\n");
    // }
    // printf("aht20 trigger successed !\r\n");

    // return (err == ESP_OK)? 0 : -1;

}

static int  _get_aht20_measurement_data(struct AHT20 *self, float *humidity, float *temp){

    _aht20_trigger_cmd_(self);
    uint8_t status_byte = 0;
    // 讀取狀態字節
    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    i2c_master_write_byte(cmd_handle, (self->dev->slave_addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd_handle, &status_byte, I2C_MASTER_NACK);
    i2c_master_stop(cmd_handle);
    i2c_master_cmd_begin(I2C_MASTER_NUM, cmd_handle, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd_handle);
    vTaskDelay(20 / portTICK_PERIOD_MS);

    uint8_t data[6];
    if((status_byte & 0X80) == 0){
        // 測量完成，可以連續讀取六個字節
        cmd_handle = i2c_cmd_link_create();
        i2c_master_start(cmd_handle);
        i2c_master_write_byte(cmd_handle, (self->dev->slave_addr << 1) | I2C_MASTER_READ, true);
        i2c_master_read(cmd_handle, data, sizeof(data), I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd_handle);
        i2c_master_cmd_begin(I2C_MASTER_NUM, cmd_handle, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd_handle);
        // printf("aht20 measurement completed !\r\n");
    }else{

        // printf("aht20 is busy ! \r\n");
        return 0;
    }
    

    if(data[0] & 0x80){
        // printf("aht20 is busy \r\n");
        return 0;
    }

    // printf("aht20 get data \r\n");

    uint32_t _temp = 0;
    uint32_t _humidity = 0;

    _humidity |= data[1];
    _humidity <<= 8;
    _humidity |= data[2];
    _humidity <<= 4;
    _humidity |= (data[3] >> 4);

    *(humidity) = (float)_humidity/1048576.0 * 100;
    // printf("humidity: %.2f \r\n", *(humidity));

    _temp = data[3] & 0x0f;
    _temp <<= 8;
    _temp |= data[4];
    _temp <<= 8;
    _temp |= data[5];

    *temp = (float)_temp/1048576.0 * 200.0 - 50.0;
    // printf("temp: %.2f \r\n", *(temp));
    return 1;
  

}

static void aht20_sampling_task(void *pvParameter){

    struct AHT20 *self = (struct AHT20*)pvParameter;

    while(1){

        _aht20_trigger_cmd_(self);
        _get_aht20_measurement_data(self, &(self->dev->humidity), &(self->dev->temperature));
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


static float get_temperature(struct AHT20 *self){

    return (self->dev->temperature);

}

static float get_humidity(struct AHT20 *self){

    return (self->dev->humidity);

}

int init_aht20(struct AHT20 *self, uint8_t slave_address){

    struct i2c_dev *ptr_dev = (struct i2c_dev *)malloc(sizeof(struct i2c_dev));
    ptr_dev->slave_addr = slave_address;
    self->dev = ptr_dev;
    // printf(" dev address:%p \r\n", ptr_dev);

    if(self == NULL | ptr_dev == NULL){
        return ESP_FAIL;
    }

    if(_aht20_init_cmd_(self) == ESP_FAIL){
        printf("error with aht20 init \r\n");
        return ESP_FAIL;
    }
    printf("successd to initialize aht20 \r\n");

    _aht20_trigger_cmd_(self);
    _get_aht20_measurement_data(self, &(self->dev->humidity), &(self->dev->temperature));

    self->get_temperature = get_temperature;
    self->get_humidity = get_humidity;
    
    xTaskCreate(aht20_sampling_task, "aht20_sampling_task", 1024 * 2, self, 1, NULL);

    return ESP_OK;
}
