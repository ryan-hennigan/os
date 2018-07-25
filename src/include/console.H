#ifndef __Console_H__
#define __Console_H__

typedef enum {
   BLACK         = 0,
   BLUE          = 1,
   GREEN         = 2,
   CYAN          = 3, 
   RED	         = 4,
   MAGENTA       = 5,
   BROWN         = 6,
   LIGHT_GREY    = 7,
   DARK_GREY     = 8,  	
   LIGHT_BLUE    = 9, 	
   LIGHT_GREEN   = 10, 	
   LIGHT_CYAN    = 11, 	
   LIGHT_RED     = 12, 	
   LIGHT_MAGENTA = 13, 	
   LIGHT_BROWN   = 14, 	
   WHITE	 = 15 	
} COLOR_CODE;

class Console
{

public:
    static void scroll(void);
    static void move_csr(void);
    static void cls();
    static void putch(const char c);
    static void puts(const char *text);
    static void settextcolor(unsigned char forecolor, unsigned char backcolor);
    static void init(void);

private:
    static unsigned short *textmemptr;
    static int attrib;
    static int csr_x, csr_y;
};

#endif




