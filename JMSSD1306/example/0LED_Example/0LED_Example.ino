#include <Arduino.h>
#include <driver/i2c.h>

extern "C"{
#include "JM_SSD1306.h"
};

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

void Task_Display(void *pvParameter)
{
  oled.clear(&oled);
  vTaskDelay(10 / portTICK_PERIOD_MS);
  oled.display_info(&oled);

  /* buffer for display character */
  const char *student_id = "N961140XX";
  char buf[50];

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  oled.clear(&oled);
  
  while(1){

    snprintf(buf, sizeof(buf), " ID: %s", student_id);
    oled.print(&oled, 1, 1, buf, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

  }

}

void setup() 
{

  Serial.begin(115200);

  /* 初始化並啟用esp32 i2c controller*/
  if(init_i2c() == ESP_OK){
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Initialize i2c successed!");
  }

  /* 初始化oled */
  if(init_oled(&oled, SDA_PIN, SCL_PIN, 0x3C) == ESP_OK){
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Initialize OLED successed!");
  }

  xTaskCreate(Task_Display, "Task_display", 1024 * 2, NULL, 1, NULL);

}

void loop() {
}

