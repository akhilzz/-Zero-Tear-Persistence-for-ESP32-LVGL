/**
 * @file lv_conf.h
 * Minimal configuration for Zero-Tear NVS Persistence Example
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0
#define LV_MEM_SIZE (128U * 1024U)
#define LV_DISP_DEF_REFR_PERIOD 20
#define LV_INDEV_DEF_READ_PERIOD 30
#define LV_DPI_DEF 130
#define LV_DRAW_COMPLEX 1
#define LV_USE_LOG 0
#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MALLOC 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14
#define LV_TXT_ENC LV_TXT_ENC_UTF8
#define LV_USE_ARC 1
#define LV_USE_BAR 1
#define LV_USE_BTN 1
#define LV_USE_LABEL 1
#define LV_USE_IMG 1
#define LV_USE_THEME_DEFAULT 1

#endif
