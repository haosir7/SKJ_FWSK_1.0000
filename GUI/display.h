#ifndef _DISPLAY_H
#define _DISPLAY_H

#ifdef __cplusplus
extern "C"
{
#endif
extern unsigned char latinfont[4096];
extern void str2image(unsigned char *str, int len, unsigned char *buffer);
extern int hz_index(unsigned char *str);
extern int is_hz(unsigned char *str);

#ifdef __cplusplus
};
#endif

#endif

