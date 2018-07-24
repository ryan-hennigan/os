/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Global function declarations and type definitions
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef int size_t;

/* MAIN.C */
//extern void *memcpy(void *dest, const void *src, size_t count);
//extern void *memset(void *dest, char val, size_t count);
//extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
//extern size_t strlen(const char *str);
//extern unsigned char inportb (unsigned short _port);
//extern void outportb (unsigned short _port, unsigned char _data);


/* MAIN.C */
extern void *memcpy(void *dest, const void *src, size_t count);
extern void *memset(void *dest, char val, size_t count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

///* CONSOLE.C */
//extern void init_video(void);
//extern void puts(unsigned char *text);
//extern void putch(unsigned char c);
//extern void cls();
//
//
///* CONSOLE.C */
//extern void init_video(void);
//extern void puts(const char *text);
//extern void putch(const char c);
//extern void cls();

#endif
