#include <Arduino.h>
#define EEZ_FOR_LVGL 1
// #include <eez-framework.h>
#include <ui/ui.h>
#include <lvgl.h>
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>
#include <esp_heap_caps.h>

// I2C bus shared by the GT911 touch controller and the CH422G IO expander
#define I2C_SDA_PIN     8
#define I2C_SCL_PIN     9
#define I2C_PORT        I2C_NUM_0
#define TOUCH_INT_PIN   4

// CH422G: 0x24 is the mode register, 0x38 drives the EXIO outputs
#define CH422G_MODE_ADDR    0x24
#define CH422G_OUTPUT_ADDR  0x38
#define CH422G_MODE_OUTPUT  0x01

// Extend IO Pin define (bit index in the CH422G output register)
#define TP_RST  (1 << 0)
#define LCD_BL  (1 << 1)
#define LCD_RST (1 << 2)
#define SD_CS   (1 << 3)

static uint8_t exio_state = 0xFF & ~(TP_RST | LCD_BL | LCD_RST);

static void exio_write(uint8_t state) {
    exio_state = state;
    uint8_t data = state;
    i2c_master_write_to_device(I2C_PORT, CH422G_OUTPUT_ADDR, &data, 1, pdMS_TO_TICKS(100));
}

static void exio_set(uint8_t mask, bool on) {
    exio_write(on ? (exio_state | mask) : (exio_state & ~mask));
}

static void board_io_init() {
    i2c_config_t cfg = {};
    cfg.mode = I2C_MODE_MASTER;
    cfg.sda_io_num = I2C_SDA_PIN;
    cfg.scl_io_num = I2C_SCL_PIN;
    cfg.sda_pullup_en = GPIO_PULLUP_ENABLE;
    cfg.scl_pullup_en = GPIO_PULLUP_ENABLE;
    cfg.master.clk_speed = 400000;
    i2c_param_config(I2C_PORT, &cfg);
    i2c_driver_install(I2C_PORT, cfg.mode, 0, 0, 0);

    uint8_t mode = CH422G_MODE_OUTPUT;
    i2c_master_write_to_device(I2C_PORT, CH422G_MODE_ADDR, &mode, 1, pdMS_TO_TICKS(100));

    // Hold INT low while releasing reset so the GT911 selects address 0x5D
    pinMode(TOUCH_INT_PIN, OUTPUT);
    digitalWrite(TOUCH_INT_PIN, LOW);
    exio_write(exio_state & ~(TP_RST | LCD_RST | LCD_BL));
    delay(100);
    exio_set(TP_RST, true);
    delay(50);
    pinMode(TOUCH_INT_PIN, INPUT);
    exio_set(LCD_RST, true);
    delay(200);
}

/* LVGL porting configurations */
#define LVGL_TICK_PERIOD_MS     (250)
#define LVGL_TASK_MAX_DELAY_MS  (500)
#define LVGL_TASK_MIN_DELAY_MS  (1)
#define LVGL_TASK_STACK_SIZE    (4 * 1024)
#define LVGL_TASK_PRIORITY      (2)
#define LVGL_BUF_SIZE           (ESP_PANEL_LCD_H_RES * ESP_PANEL_LCD_V_RES / 10)

static ESP_Panel *panel = nullptr;

static void display_flush(lv_disp_drv_t *display, const lv_area_t *area, lv_color_t *colors) {
    panel->getLcd()->drawBitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, colors);
    lv_disp_flush_ready(display);
}

#if ESP_PANEL_USE_LCD_TOUCH
static void touch_read(lv_indev_drv_t *input, lv_indev_data_t *data) {
    (void)input;
    panel->getLcdTouch()->readData();
    if (!panel->getLcdTouch()->getTouchState()) {
        data->state = LV_INDEV_STATE_REL;
        return;
    }

    const TouchPoint point = panel->getLcdTouch()->getPoint();
    data->state = LV_INDEV_STATE_PR;
    data->point.x = point.x;
    data->point.y = point.y;
}
#endif

void setup() {
    Serial.begin(115200);
    Serial.println("[System] Initializing Waveshare 4.3 LCD...");

    panel = new ESP_Panel();

    board_io_init();

    lv_init();

    static lv_disp_draw_buf_t draw_buffer;
    lv_color_t *buffer = static_cast<lv_color_t *>(
        heap_caps_calloc(LVGL_BUF_SIZE, sizeof(lv_color_t), MALLOC_CAP_INTERNAL));
    if (buffer == nullptr) {
        Serial.println("[Error] Unable to allocate LVGL draw buffer.");
        while (true) {
            delay(1000);
        }
    }
    lv_disp_draw_buf_init(&draw_buffer, buffer, nullptr, LVGL_BUF_SIZE);

    static lv_disp_drv_t display_driver;
    lv_disp_drv_init(&display_driver);
    display_driver.hor_res = ESP_PANEL_LCD_H_RES;
    display_driver.ver_res = ESP_PANEL_LCD_V_RES;
    display_driver.flush_cb = display_flush;
    display_driver.draw_buf = &draw_buffer;
    lv_disp_drv_register(&display_driver);

#if ESP_PANEL_USE_LCD_TOUCH
    static lv_indev_drv_t input_driver;
    lv_indev_drv_init(&input_driver);
    input_driver.type = LV_INDEV_TYPE_POINTER;
    input_driver.read_cb = touch_read;
    lv_indev_drv_register(&input_driver);
#endif

    panel->init();
    panel->begin();
    ui_init();
    exio_set(LCD_BL, true);
}

void loop() {
    lv_tick_inc(5);
    lv_timer_handler();
    ui_tick();
    delay(5);
}
