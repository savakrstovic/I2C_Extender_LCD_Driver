
#include <project.h>


#ifndef FDB_LIQUID_CRYSTAL_I2C_H
#define FDB_LIQUID_CRYSTAL_I2C_H

/* commands */
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20  /* Already set for 4 bits */
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

/* flags for display entry mode*/
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

/* flags for display on/off control */
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

/* flags for display/cursor shift */
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
    
/* LCD Conversion Routines  */    
    
#define LCD_NUMBER_OF_REMAINDERS_U32 (0x0Au)
#define LCD_TEN                      (0x0Au)
#define LCD_8_BIT_SHIFT              (8u)
#define LCD_32_BIT_SHIFT             (32u)
#define LCD_ZERO_CHAR_ASCII          (48u)    

/* flags for function set */
#define LCD_8BITMODE 0x10   /* Interface data length control bit */
#define LCD_4BITMODE 0x00   /* Interface data length control bit */
#define LCD_2LINE 0x08      /* Display line number control bit */
#define LCD_1LINE 0x00      /* Display line number control bit */
#define LCD_5x10DOTS 0x04   /* Display font type control bit */
#define LCD_5x8DOTS 0x00    /* Display font type control bit */

/* flags for backlight control */
#define LCD_BACKLIGHT 0x08   /* P3 port is connected to cathode through transistor */
#define LCD_NOBACKLIGHT 0x00

#define En 0x04  /* Enable bit */
#define Rw 0x02  /*  Read/Write bit; 0 is for write, 1 is for read */
#define Rs 0x01  /* Register select bit; 0: Instruction register (for write) Busy flag:address counter (for read) 
                  1: Data register (for write and read)  Data register is picked with this value */
#define Rs_low 0x00
#define En_low 0x00
#define Rs_IR  0x00    
/**
 * This is the driver for the Liquid Crystal LCD displays that uses the I2C bus with PCF8574AT (16 pin SMD version) general purpose remote I/O expansion.
 *
 * After creating an instance of this class, first call begin() before anything else.
 * The backlight is on by default, since that is the most likely operating mode in
 * most cases.
 */

/*
 * Constructor
 *
 * @param lcd_addr	I2C slave address of the LCD display. Most likely printed on the
 *					LCD circuit board, or look in the supplied LCD documentation.
 * @param lcd_cols	Number of columns your LCD display has.
 * @param lcd_rows	Number of rows your LCD display has.
 * @param charsize	The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
*/

    
    
/* List of function prototypes */    
    
void LiquidCrystal_I2C_init(uint8 lcd_addr, uint8 lcd_cols, uint8 lcd_rows, uint8 charsize);
	
void I2C_M_write_byte(uint8 addr, uint8 data);

/*
 * Set the LCD display in the correct begin state, must be called before anything else is done.
*/
void LCD_Start_4bit(void);
	
/*
 * Remove all the characters currently shown. Next print/write operation will start
 * from the first position on LCD display.
*/
void clear(void);
	  
/*
 * Next print/write operation will will start from the first position on the LCD display.
*/
void home(void);

/*
 * Do not show any characters on the LCD display. Backlight state will remain unchanged.
 * Also all characters written on the display will return, when the display in enabled again.
*/
void noDisplay(void);
	  
/*
 * Show the characters on the LCD display, this is the normal behaviour. This method should
 * only be used after noDisplay() has been used.
*/ 
void display(void);

/*
 * Do not blink the cursor indicator.
*/
void noBlink(void);
	 
/*
 * Start blinking the cursor indicator.
*/ 
void blink(void);	 

/*
 * Do not show a cursor indicator.
*/
void noCursor(void);

/*
 * Show a cursor indicator, cursor can blink on not blink. Use the
 * methods blink() and noBlink() for changing cursor blink.*/ 
void cursor(void);

void scrollDisplayLeft(void);
void scrollDisplayRight(void);
void printLeft(void);
void printRight(void);
void leftToRight(void);
void rightToLeft(void);
void shiftIncrement(void);
void shiftDecrement(void);
void noBacklight(void);
void backlight(void);
void autoscroll(void);
void noAutoscroll(void); 
void createChar(uint8, uint8[]);
void setCursor(uint8 row, uint8 col); 
void write(uint8);
void command(uint8);        
void LCD_print(char word[]);
void LCD_PutChar(char8 character);
void LCD_PrintInt8(uint8 value);
void LCD_PrintInt16(uint16 value);
void LCD_PrintU32Number(uint32 value);
void LCD_PrintNumber(uint16 value); 


// Compatibility API function aliases
void setBacklight(uint8 new_val);				// alias for backlight() and nobacklight()
void load_custom_character(uint8 char_num, uint8 *rows);	// alias for createChar()	 
void send(uint8, uint8);
void write4bits(uint8);
void extenderWrite(uint8);
void pulseEnable(uint8);

/* Global variables */

extern uint8 _addr;
extern uint8 _displayfunction;
extern uint8 _displaycontrol;
extern uint8 _displaymode;
extern uint8 _cols;
extern uint8 _rows;
extern uint8 _charsize;
extern uint8 _backlightval;

#define LCD_PrintNumber(value)  LCD_PrintU32Number((uint16) (value))
#define LCD_PrintDecUint16(x)   LCD_PrintNumber(x)

#endif  //  FDB_LIQUID_CRYSTAL_I2C_H