#pragma once
//FEATURES
#define TAPPING_TOGGLE 2
#define GRAVE_ESC_CTRL_OVERRIDE
#define UNICODE_SELECTED_MODES UNICODE_MODE_WINCOMPOSE, UNICODE_MODE_LINUX
//--ENCODER
#define ENCODER_RESOLUTION 4
//--OLED
#ifdef OLED_ENABLE
  #define OLED_TIMEOUT 0
  #define OLED_BRIGTHNESS 100
  #define OLED_FONT_H "keyboards/nullbitsco/nibble/keymaps/BiCH0/glcdfont.c"
#endif
