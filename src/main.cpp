#include <Arduino.h>
#define EEZ_FOR_LVGL 1
// #include <eez-framework.h>
#include <ui/ui.h>
#include <lvgl.h>
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>

// I2C Pins dedicated to the touch driver & CH422G expander
#define I2C_SDA_PIN     17
#define I2C_SCL_PIN     18
// I2C Pin define
#define I2C_MASTER_NUM 0
#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_SCL_IO 9

ESP_IOExpander *expander = new ESP_IOExpander_TCA95xx_8bit(I2C_NUM_0, ESP_IO_EXPANDER_I2C_TCA9554_ADDRESS_000, I2C_SCL_PIN, I2C_SDA_PIN);

// Extend IO Pin define
#define TP_RST 1
#define LCD_BL 2
#define LCD_RST 3
#define SD_CS 4
#define USB_SEL 5

/* LVGL porting configurations */
#define LVGL_TICK_PERIOD_MS     (250)
#define LVGL_TASK_MAX_DELAY_MS  (500)
#define LVGL_TASK_MIN_DELAY_MS  (1)
#define LVGL_TASK_STACK_SIZE    (4 * 1024)
#define LVGL_TASK_PRIORITY      (2)
#define LVGL_BUF_SIZE           (ESP_PANEL_LCD_H_RES * ESP_PANEL_LCD_V_RES / 10)

void setup() {
    // 1. Initialize Display and Touch Drivers
    Serial.begin(115200);
    Serial.println("[System] Initializing Waveshare 4.3 LCD...");

    
    // 1b. Wake up the CH422G IO Expander to enable display signals
    // Waveshare's customized driver handles expander addressing over the shared I2C bus
    expander->init();
    expander->begin();

    // 1c. Fire up the RGB LCD Bus (ST7262 Driver running at 800x480)
    Serial.println("[Hardware] Booting RGB panel bus...");
    waveshare_lcd_init(); 

    // 1d. Assert power to display backlight
    // Pin maps vary slightly across hardware iterations (A, B, C), 
    // but backlight is uniformly toggled via the expander rail.
    Serial.println("[System] Initialization Sequence Completed.");
    // 2. Initialize LVGL (lv_init())
    lv_init();
    // 3. Initialize EEZ Studio UI
    ui_init(); 
}

void loop() {
    lv_timer_handler(); // Let LVGL process graphics
    ui_tick();          // Updates internal variables/flows mapped in EEZ Studio
    delay(5);
}

