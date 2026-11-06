/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "soc/soc_caps.h"

#if SOC_LCD_RGB_SUPPORTED
#include <stdlib.h>
#include <string.h>

#include "esp_lcd_panel_io.h"

#include "private/CheckResult.h"
#include "RGB.h"

// ESP-IDF versions that expose the frame-buffer completion callback use this
// field name in the RGB panel configuration.
#define on_frame_trans_done on_frame_buf_complete
#define esp_lcd_rgb_panel_frame_trans_done_cb_t esp_lcd_rgb_panel_frame_buf_complete_cb_t

static const char *TAG = "ESP_PanelBus_RGB";

static esp_lcd_rgb_panel_config_t makeRGBConfig(uint16_t width, uint16_t height, int hsync, int vsync, int pclk,
                                                 const int *data_pins, size_t data_width, int de, int disp)
{
    esp_lcd_rgb_panel_config_t config = {};
    config.timings.pclk_hz = 16 * 1000 * 1000;
    config.timings.h_res = width;
    config.timings.v_res = height;
    config.timings.hsync_pulse_width = 4;
    config.timings.hsync_back_porch = 8;
    config.timings.hsync_front_porch = 8;
    config.timings.vsync_pulse_width = 4;
    config.timings.vsync_back_porch = 8;
    config.timings.vsync_front_porch = 8;
    config.data_width = data_width;
    config.bits_per_pixel = data_width;
    config.hsync_gpio_num = hsync;
    config.vsync_gpio_num = vsync;
    config.pclk_gpio_num = pclk;
    config.de_gpio_num = de;
    config.disp_gpio_num = disp;
    for (size_t i = 0; i < data_width; ++i) {
        config.data_gpio_nums[i] = data_pins[i];
    }
    return config;
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, const esp_lcd_panel_io_3wire_spi_config_t *spi_config, int host_id)
    : ESP_PanelBus(host_id)
{
    if (rgb_config == nullptr || spi_config == nullptr) {
        ESP_LOGE(TAG, "Check NULL");
        return;
    }

    memcpy(&this->rgb_config, rgb_config, sizeof(this->rgb_config));
    this->spi_config = *spi_config;
    flags.host_need_init = true;
}

/// @brief 
/// @param rgb_config 
/// @param host_id 
ESP_PanelBus_RGB::ESP_PanelBus_RGB(const esp_lcd_rgb_panel_config_t *rgb_config, int host_id)
    : ESP_PanelBus(host_id)
{
    if (rgb_config == nullptr) {
        ESP_LOGE(TAG, "Check NULL");
        return;
    }

    memset(&this->rgb_config, 0, sizeof(this->rgb_config));
    memcpy(&this->rgb_config, rgb_config, sizeof(this->rgb_config));
    flags.host_need_init = false;
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height,  int hsync, int vsync, int pclk,
                                   int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7,
                                   int D8, int D9, int D10, int D11, int D12, int D13, int D14, int D15,
                                   int de, int disp, int cs, int scl, int sda):
    rgb_config(makeRGBConfig(width, height, hsync, vsync, pclk,
               (const int[]){D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, D10, D11, D12, D13, D14, D15}, 16, de, disp)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)RGB_PANEL_IO_CONFIG_DEFAULT(cs, scl, sda))
{
    flags.host_need_init = ((cs != -1) && (scl != -1) && (sda != -1));
}

ESP_PanelBus_RGB::ESP_PanelBus_RGB(uint16_t width, uint16_t height, int hsync, int vsync, int pclk,
                                   int D0, int D1, int D2, int D3, int D4, int D5, int D6, int D7,
                                   int de, int disp, int cs, int scl, int sda):
    rgb_config(makeRGBConfig(width, height, hsync, vsync, pclk,
               (const int[]){D0, D1, D2, D3, D4, D5, D6, D7}, 8, de, disp)),
    spi_config((esp_lcd_panel_io_3wire_spi_config_t)RGB_PANEL_IO_CONFIG_DEFAULT(cs, scl, sda))
{
    flags.host_need_init = ((cs != -1) && (scl != -1) && (sda != -1));
}

ESP_PanelBus_RGB::~ESP_PanelBus_RGB()
{
    if (handle && flags.host_need_init) {
        del();
    }
}

const esp_lcd_rgb_panel_config_t *ESP_PanelBus_RGB::getRGBConfig()
{
    return &rgb_config;
}

void ESP_PanelBus_RGB::init(void)
{
    if (flags.host_need_init) {
        CHECK_ERROR_RETURN(esp_lcd_new_panel_io_3wire_spi(&spi_config, &handle));
    }
}

#endif /* SOC_LCD_RGB_SUPPORTED */

#undef on_frame_trans_done
#undef esp_lcd_rgb_panel_frame_trans_done_cb_t
