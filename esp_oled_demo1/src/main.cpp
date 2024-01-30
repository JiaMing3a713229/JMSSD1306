#include <Arduino.h>
#include <driver/i2c.h>
#ifdef __cplusplus
extern "C"{
  #include "JM_SSD1306.h"
}
#endif



#define SDA_PIN 21
#define SCL_PIN 22

OLED oled;

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



void displayTask(void *pvParameter){

  vTaskDelay(100 / portTICK_PERIOD_MS);
  oled.drawLogo(&oled);
  oled.clear(&oled);
  int count = 0;
  char buffer[50];

  while(1){

    snprintf(buffer, sizeof(buffer), " Count: %d", count);
    oled.print(&oled, 1, 1, buffer, 0);
    count++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }
}


void setup() {

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.begin(115200);

  //初始化並且啟用ESP32 I2C通訊介面
  if(init_i2c() == ESP_OK){
    Serial.println("i2c initialize sucessed");
  }
  
  //初始化OLED暫存器配置
  init_oled(&oled, SDA_PIN, SCL_PIN, 0x3c);
  if((&oled) == NULL){
    printf("\nInitialize OLED error !\r\n");
  }

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  //建立並初始化顯示工作的Threads
  xTaskCreate(displayTask, "TaskforDisplay", 1024 * 2, NULL, 1, NULL);

}

void loop() {
  
}

