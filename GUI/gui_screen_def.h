#ifndef GUI_SCREEN_DEF_H
#define GUI_SCREEN_DEF_H
#include "lcd_tiny.h"

#if CHN_FONT_SIZE == 16
#define WORD_W	16
#define WORD_H	16
#define CHAR_W	8
#define CHAR_H	16
#define LABEL_H	16
#else
#define WORD_W		12
#define WORD_H		12
#define CHAR_W		6
#define CHAR_H		12
#define LABEL_H		12
#endif

#endif //GUI_SCREEN_DEF_H
