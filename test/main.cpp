#include <Arduino.h>

#include <lvgl.h>

// Widget declarations
static lv_obj_t *twinkle;
static lv_obj_t *meteor;
static lv_obj_t *red;
static lv_obj_t *page_switch;
static lv_obj_t *page_switch2;
static lv_obj_t *lv_button_14;

// Colors
#define BG_COLOR lv_color_hex(0x111827)
#define WHITE_COLOR lv_color_hex(0xFFFFFF)
#define RED_COLOR lv_color_hex(0xE00000)
#define GREEN_COLOR lv_color_hex(0x07E000)

// Navigation callbacks
static void page_left_event(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // Add previous page logic here
    }
}

static void page_right_event(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // Add next page logic here
    }
}

static void button_event(lv_event_t *e)
{
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        // bool state = lv_obj_has_state(
        //     lv_event_get_target(e), LV_STATE_CHECKED
        // );

        // Handle button ON/OFF
    }
}

void create_canvas_1(lv_obj_t *parent)
{
    // Screen background
    lv_obj_set_style_bg_color(parent, BG_COLOR, 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);
    lv_obj_set_style_pad_all(parent, 0, 0);
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);

    // -------------------------
    // TWINKLE DROPDOWN
    // -------------------------

    twinkle = lv_dropdown_create(parent);

    lv_dropdown_set_options(
        twinkle,
        "speed 1\n"
        "speed 2\n"
        "speed 3\n"
        "speed 4"
    );

    lv_dropdown_set_selected(twinkle, 0);

    lv_obj_set_pos(twinkle, 125, 60);
    lv_obj_set_size(twinkle, 150, 40);

    lv_obj_set_style_bg_color(twinkle,
        lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_set_style_border_width(twinkle, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(twinkle, WHITE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(twinkle, WHITE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(twinkle, WHITE_COLOR, LV_PART_SELECTED);

    // -------------------------
    // METEOR DROPDOWN
    // -------------------------

    meteor = lv_dropdown_create(parent);

    lv_dropdown_set_options(
        meteor,
        "meteor 1\n"
        "meteor 2\n"
        "meteor 3\n"
        "meteor 4\n"
        "meteor 5\n"
        "meteor 6\n"
        "meteor 7\n"
        "meteor 8\n"
        "meteor 9\n"
        "meteor 10\n"
        "meteor 11\n"
        "meteor 12\n"
        "meteor 13\n"
        "meteor 14\n"
        "meteor 15\n"
        "meteor 16\n"
        "meteor 17\n"
        "meteor 18\n"
        "meteor19\n"
        "meteor 20"
    );

    lv_dropdown_set_selected(meteor, 0);

    lv_obj_set_pos(meteor, 125, 135);
    lv_obj_set_size(meteor, 150, 40);

    lv_obj_set_style_bg_color(meteor,
        lv_color_hex(0x000000), LV_PART_MAIN);

    lv_obj_set_style_border_width(meteor, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(meteor, WHITE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(meteor, WHITE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_text_color(meteor, WHITE_COLOR, LV_PART_SELECTED);

    // -------------------------
    // LEFT NAVIGATION BUTTON
    // -------------------------

    page_switch = lv_btn_create(parent);

    lv_obj_set_pos(page_switch, 0, 0);
    lv_obj_set_size(page_switch, 110, 480);

    lv_obj_set_style_bg_color(page_switch, BG_COLOR, 0);
    lv_obj_set_style_shadow_width(page_switch, 2, 0);
    lv_obj_set_style_shadow_color(page_switch, BG_COLOR, 0);
    lv_obj_set_style_shadow_opa(page_switch, 50, 0);
    lv_obj_set_style_shadow_ofs_y(page_switch, 2, 0);

    lv_obj_t *left_label = lv_label_create(page_switch);
    lv_label_set_text(left_label, "<");
    lv_obj_center(left_label);

    lv_obj_add_event_cb(
        page_switch, page_left_event, LV_EVENT_CLICKED, NULL
    );

    // -------------------------
    // RIGHT NAVIGATION BUTTON
    // -------------------------

    page_switch2 = lv_btn_create(parent);

    lv_obj_set_pos(page_switch2, 690, 0);
    lv_obj_set_size(page_switch2, 110, 480);

    lv_obj_set_style_bg_color(page_switch2, BG_COLOR, 0);
    lv_obj_set_style_radius(page_switch2, 13, 0);
    lv_obj_set_style_shadow_width(page_switch2, 2, 0);
    lv_obj_set_style_shadow_color(page_switch2, BG_COLOR, 0);
    lv_obj_set_style_shadow_opa(page_switch2, 50, 0);
    lv_obj_set_style_shadow_ofs_y(page_switch2, 2, 0);

    lv_obj_t *right_label = lv_label_create(page_switch2);
    lv_label_set_text(right_label, ">");
    lv_obj_center(right_label);

    lv_obj_add_event_cb(
        page_switch2, page_right_event, LV_EVENT_CLICKED, NULL
    );

    // -------------------------
    // RED VERTICAL SLIDER
    // -------------------------

    red = lv_slider_create(parent);

    lv_obj_set_pos(red, 390, 70);
    lv_obj_set_size(red, 20, 200);

    lv_slider_set_range(red, 0, 100);
    lv_slider_set_value(red, 56, LV_ANIM_OFF);

    lv_obj_set_style_bg_color(red, RED_COLOR, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(red, 51, LV_PART_MAIN);
    lv_obj_set_style_radius(red, LV_RADIUS_CIRCLE, LV_PART_MAIN);

    lv_obj_set_style_bg_color(red, RED_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(red, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_radius(red, LV_RADIUS_CIRCLE, LV_PART_INDICATOR);

    lv_obj_set_style_bg_color(red, RED_COLOR, LV_PART_KNOB);
    lv_obj_set_style_bg_opa(red, LV_OPA_COVER, LV_PART_KNOB);

    // -------------------------
    // GREEN CHECKABLE BUTTON
    // -------------------------

    lv_button_14 = lv_btn_create(parent);

    lv_obj_set_pos(lv_button_14, 150, 300);
    lv_obj_set_size(lv_button_14, 100, 40);

    lv_obj_add_flag(lv_button_14, LV_OBJ_FLAG_CHECKABLE);

    lv_obj_set_style_bg_color(
        lv_button_14, GREEN_COLOR, LV_PART_MAIN
    );

    lv_obj_set_style_text_color(
        lv_button_14, RED_COLOR, LV_PART_MAIN
    );

    lv_obj_add_event_cb(
        lv_button_14, button_event, LV_EVENT_VALUE_CHANGED, NULL
    );
}

// Call after lv_init() and display initialization
void setup_ui()
{
    create_canvas_1(lv_scr_act());
}




