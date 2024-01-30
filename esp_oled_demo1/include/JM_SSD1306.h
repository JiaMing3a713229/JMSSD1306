#ifndef _JM_SSD1306_H_
#define _JM_SSD1306_H_

#include "Arduino.h"

/*SSD1306 Command List*/
#define SSD1306_MEMORYMODE 0x20          ///< Set Memory Addressing Mode (20h) 
#define SSD1306_COLUMNADDR 0x21          ///< Set Column Address (21h)
#define SSD1306_PAGEADDR 0x22            ///< Set Page Address (22h)
#define SSD1306_SETCONTRAST 0x81         ///< Set Contrast Control for BANK0 (81h) 
#define SSD1306_SEGREMAP 0xA0            ///< Set Segment Re-map
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< A4h command enable display outputs according to the GDDRAM contents. 
#define SSD1306_DISPLAYALLON 0xA5        ///< A5h command forces the entire display to be “ON”
/*Set Normal/Inverse Display (A6h/A7h) */
#define SSD1306_NORMALDISPLAY 0xA6       ///< Normal display a RAM data of 1 indicates an “ON” pixel
#define SSD1306_INVERTDISPLAY 0xA7       ///< Inverse display a RAM data of 0 indicates an “ON” pixel.

#define SSD1306_SETMULTIPLEX 0xA8        ///< Set Multiplex Ratio (A8h)

#define SSD1306_DISPLAYOFF 0xAE          ///< Set Display ON
#define SSD1306_DISPLAYON 0xAF           ///< Set Display OFF

#define SSD1306_COMSCANINC 0xC0          ///< Set COM Output Scan Direction
#define SSD1306_COMSCANDEC 0xC8          ///< Set COM Output Scan Direction

#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< Set Display Offset
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< Set Display Clock Divide Ratio

#define SSD1306_SETPRECHARGE 0xD9        ///< Set Pre-charge Period
#define SSD1306_SETCOMPINS 0xDA          ///< Set COM Pins Hardware Configuration (DAh)
#define SSD1306_SETVCOMDETECT 0xDB       ///< Set VCOMH Deselect Level (DBh) 

#define SSD1306_SETLOWCOLUMN 0x00        ///< Set the lower 4 bits of the COL address
#define SSD1306_SETHIGHCOLUMN 0x10       ///< Set the higher 4 bits of the COL address

#define SSD1306_SETSTARTLINE 0x40        ///< Set Display Start Line
#define SSD1306_CHARGE_PUMP 0x8D         ///< Charge Pump Setting


#define I2C_MASTER_TIMEOUT_MS 1000      

// #define SSD1306_SLAVE_ADDR 0x3C       ///< set slave address of ssd1306 to 0x3C or 0x3D

#define CMD_MODE 0
#define DATA_MODE 1


#define I2C_MASTER_NUM 0                                /*!< esp32內部有2個I2C控制器，我們使用第1個(0)控制器*/
#define I2C_MASTER_FREQ_HZ          400000              /*!< I2C master clock frequency，400KHz */
#define I2C_MASTER_TX_BUF_DISABLE   0                   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                   /*!< I2C master doesn't need buffer */

typedef enum{
    FONT_SIZE_F6x8 = 0,
    FONT_SIZE_F8X16 = 1
}font_size;

typedef struct{

    int sda_io_num;
    int scl_io_num;
    uint8_t ssd1306_slave_addr;
    
}SSD1306_t;
typedef struct{

    SSD1306_t *ssd1306;
    //Declare function pointer
    // void *(*init_OLED)(int SDA_PIN, int SCL_PIN, uint8_t ssd1306_slave_addr);
    void (*set_pos)(void *self, uint8_t x, uint8_t y);
    void (*print)(void *self, uint8_t x, uint8_t y, const char* msg, uint8_t font_size);
    void (*clear)(void *self);
    void (*drawLogo)(void *self);

}OLED;


int init_oled(OLED *self, int SDA_PIN, int SCL_PIN, uint8_t ssd1306_slave_addr);
// static void set_pos(void *self, uint8_t x, uint8_t y);
// static void print(void *self, uint8_t x, uint8_t y, const char* msg, uint8_t font_size);
// static void clear(void *self);
// static void esp_test(void);
// static void displayLogo(void);
// static void drawLogo(void);
#endif  //_JM_ssd1306_H_