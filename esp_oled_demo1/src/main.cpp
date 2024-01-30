#include <Arduino.h>
#include <driver/i2c.h>
#ifdef __cplusplus
extern "C"{
  #include "JM_SSD1306.h"
}
#endif



#define SDA_PIN 21
#define SCL_PIN 22

static esp_err_t i2c_master_init(int sda_io_num, int scl_io_num){

    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_io_num,
        .scl_io_num = scl_io_num,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
          .clk_speed = I2C_MASTER_FREQ_HZ,
        }
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static esp_err_t init_i2c(void){

  while(i2c_master_init(SDA_PIN, SCL_PIN) != ESP_OK){

        vTaskDelay(20 / portTICK_PERIOD_MS);

  }
  return ESP_OK;

}

OLED oled;

void setup() {

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.begin(115200);
  if(init_i2c() == ESP_OK){
    Serial.println("i2c initialize sucessed");
  }
  
  init_oled(&oled, SDA_PIN, SCL_PIN, 0x3c);
  if((&oled) == NULL){
    printf("\nInitialize OLED error !\r\n");
  }
  printf("address ssd1306 %p \r\n", (oled.ssd1306));

  oled.drawLogo(&oled, 0, 0);
  
  // printf("SDA:%d, SCL %d ", (oled.ssd1306)->sda_io_num, (oled.ssd1306)->scl_io_num);
  // printf("(1) Address of OLED %p (2) SLAVE ADDRESS 0x%x \r\n", &oled, oled.ssd1306->ssd1306_slave_addr);



}

void loop() {
  
}

