**JMSSD1306 (附程式庫)**
===
#### 此程式用於驅動SSD1306並且控制OLED顯示字串

## 預備作業
### 下載JM_SSD1306程式庫
#### Github:
[https://github.com/JiaMing3a713229/JMSSD1306](https://github.com/JiaMing3a713229/JMSSD1306)
![image](https://hackmd.io/_uploads/H12WxXU5p.png)

#### 將JM_SSD1306添加至Arduino中
![image](https://hackmd.io/_uploads/Hk1RlmI9p.png)

---
## 引入標頭檔，使用JM_SSD1306
```c
#include <Arduino.h>
#include <driver/i2c.h>
#ifdef __cplusplus
extern "C"{
  #include "JM_SSD1306.h"
}
#endif
```
---

## Step 1 啟用ESP32 I2C Interface
```c

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
```

## Example 顯示文字
```c
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


```


---
### 藉由上述範例，我們可以用print來顯示字串於oled上，如果想了解更多功能也可以參考程式庫檔案，自行設計屬於自己的Oled程式庫。

## **JM_SSD1306 API**
#### JM_SSD1306提供驅動SSD1306控制0.96吋OLED顯示文字的函式。

## init_oled
#### This function is used to initialize SSD1306 register to drive 0.96 OLED

```c
init_oled(OLED *self, int SDA_PIN, int SCL_PIN, uint8_t ssd1306_slave_addr);
```
#### This function will return ESP_OK(1) if the SSD1306 initialize successed.
* **self**: Point to address of OLED Instance
* **SDA_PIN** sets the GPIO to be used as the I2C peripheral data line
* **SCL_PIN** sets the GPIO to be used as the I2C peripheral clock line
* **ssd1306_slave_addr** Under I2C, the device's slave address

---

## **print**
#### This function is used to display a string on the OLED at the (x, y) position

```c
void print(void *self, uint8_t x, uint8_t y, const char* msg, uint8_t font_size);
```

* ** self**: Point to address of OLED Instance.
* **uint8_t x** set position x
* **uint8_t y** set position y
* **msg** String to display
* **font_size** set size of string
---
## **clear**
#### This is used to clear OLED display
```c
void clear(void *self);
```
* ** self**: Point to address of OLED Instance.

---

## **close**
#### This is used to free memory
```c
void close(void *self);
```
