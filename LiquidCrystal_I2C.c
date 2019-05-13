#include "LiquidCrystal_I2C.h"


//Portado para PSoC por Šarūnas Straigis
//http://www.element14.com/community/people/sarunaszx/blog
//Portado y modificado para PSoC 4 y PSoC 5LP por Brayan Bermudez y Carlos Diaz
//http://digitalprojectsudistrital.blogspot.com.co/



// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data and is in instruction register
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1
//    S = 0; No shift 
//
// Note, however, that resetting the Arduino doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// LiquidCrystal constructor is called).

uint8 _addr;  /* I2C slave address */
uint8 _displayfunction;
uint8 _displaycontrol;
uint8 _displaymode;
uint8 _cols;
uint8 _rows;
uint8 _charsize;
uint8 _backlightval;



/* Assigning values to function variables so the begin function would have any meaning */

void LiquidCrystal_I2C_init(uint8 lcd_addr, uint8 lcd_cols, uint8 lcd_rows, uint8 charsize)
{
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;  /* Backlight is on */
    
    return;
}


/* Function which initializes 4-bit mode */
void LCD_Start_4bit(void){
  
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;

	if (_rows > 1)
    
    {
		_displayfunction |= LCD_2LINE;
	}

	/* for some 1 line displays you can select a 10 pixel high font */
	if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

	/* SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	 according to datasheet, we need at least 40ms after power rises above 2.7V
	 before sending commands. Arduino can turn on way before 4.5V so we'll wait 50ms */
	CyDelay(50); 

	/* Now we pull both RS and R/W low to begin commands */
	extenderWrite(_backlightval);	/* reset extender and turn backlight off? (Bit 8 =1) */
	CyDelay(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46

	// we will start in 8bit mode, and try to set 4 bit mode
	write4bits(0x03 << 4);
	CyDelayUs(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	CyDelayUs(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4); 
	CyDelayUs(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4); 

	// set number of lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);  
	
	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();
	
	// clear it off
	clear();
	
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);
	
	home();
    
    return;
}  /* End begin */

/********** high level commands, for the user! */
void clear(void){
    
	command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	CyDelay(2);  // this command takes a long time!
    
    return;
}

void home(void){
	
    command(LCD_RETURNHOME);  // set cursor position to zero
	CyDelay(2);  // this command takes a long time!
    
    return;
}

void setCursor(uint8 row, uint8 col)

{
    
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _rows) 
    {
		row = _rows-1;    // we count rows starting w/0
	}
	command( LCD_SETDDRAMADDR | (col + row_offsets[row]) );
    
    return;
}

// Turn the display on/off (quickly)
void noDisplay(void){
	
    _displaycontrol &= LCD_DISPLAYOFF;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    return;
}

/* Turn on the display */
void display(void){
    
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    return;
}

// Turns the underline cursor on/off
void noCursor(void){
	
    _displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    return;
}
void cursor(void){
	
    _displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    return;
}

// Turn on and off the blinking cursor
void noBlink(void){
	
    _displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    return;
}
void blink(void){
    
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
    
    return;
}

/* These commands scroll the display without changing the RAM */
void scrollDisplayLeft(void){
    
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
    
    return;
}
void scrollDisplayRight(void){
	
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    
    return;
}

/* This is for text that flows Left to Right */
void leftToRight(void){
    
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
    
    return;
}

/* This is for text that flows Right to Left */
void rightToLeft(void){
    
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
    
    return;
}

/* This will 'right justify' text from the cursor */
void autoscroll(void){
	
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
    
    return;
}

/* This will 'left justify' text from the cursor */
void noAutoscroll(void){
	
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
    
    return;
}

/* Allows us to fill the first 8 CGRAM locations 
 with custom characters */
void createChar(uint8 location, uint8 charmap[]){ uint32_t i = 0;
	
    location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (i=0; i<8; i++) {
		write(charmap[i]);
	}
    
    return;
}

/* Turn the (optional) backlight off/on */
void noBacklight(void)

{
	
    _backlightval=LCD_NOBACKLIGHT;
	extenderWrite(0); /* In the extender function _data is OR-ed with _backlightval parameter and 0 is a command parameter to write data */
    
    return;
}

void backlight(void)

{
    
	_backlightval=LCD_BACKLIGHT;
	extenderWrite(0);
    
    return;
}

/*********** mid level commands, for sending data/cmds */


void command(uint8 value)

{
	
    send(value, Rs_low);
    
    return;
}


/* Data is sent on data register */


 void write(uint8 value)

{
    
	send(value, Rs);
    
    return;
}


/************ low level data pushing commands **********/

/* write either command or data */

void send(uint8 value, uint8 mode)

{
    
	uint8 highnib = value & 0xf0u;   /* Takes 4 bits out of byte with & operator */
	
    uint8 lownib = (value << 4) & 0xf0;  
	
    write4bits( (highnib) | mode ); /* Sends the high 4 nibs write instruction  */
	
    write4bits( (lownib) | mode ); /* Sends the low 4 nibs write instruction  */
    
    return;
}

void write4bits(uint8 value)

{
    
//	extenderWrite(value);
	pulseEnable(value);
    
    return;
}


/* Send command to expander to write down data */

void extenderWrite(uint8 _data)

{     
    
	I2C_M_write_byte( _addr, _data | _backlightval );	   
    
    return;
}

/* Mask the _data with En */

void pulseEnable(uint8 _data)

{
    
	extenderWrite(_data | En);	/* En high */
	CyDelayUs(1);		/* enable pulse must be >450ns */
	extenderWrite(_data & En_low);	/* En low */
	CyDelayUs(50);		/* commands need > 37us to settle */
    
    return;
}

void load_custom_character(uint8 char_num, uint8 *rows)

{
	
    createChar(char_num, rows);
    
    return;
}

/* Writes a single character to the current cursor position of the LCD module. */

void LCD_PutChar(char8 character)
{
    write((uint8)character);
}

/* Print a byte as two ASCII characters. */

void LCD_PrintInt8(uint8 value) 
{
    static char8 const CYCODE LCD_hex[16u] = "0123456789ABCDEF";
    
    LCD_PutChar((char8) LCD_hex[value >> 0x04u ]);
    LCD_PutChar((char8) LCD_hex[ value & 0x0Fu ]);
}
/* Print a uint16 as four ASCII characters. */

void LCD_PrintInt16(uint16 value) 
    {
        LCD_PrintInt8((uint8)(value >> 0x08u));
        LCD_PrintInt8((uint8)(value & 0xFFu));
    }

void setBacklight(uint8 new_val)

{
    
	if (new_val) {
		backlight();		/* turn backlight on */
	} else {
		noBacklight();		/* turn backlight off */
	}
    
    return;
}


/* Print string on LCD screen */

void LCD_print(char word[])

{ 
    
    uint32 i = 0;
    
       
    uint8 size = strlen(word);
    
    for (i = 0; i < size; i++){
        write(word[i]);
    }

    return;
}

/*******************************************************************************
    *  Function Name: LCD_PrintU32Number
    ********************************************************************************
    *
    * Summary:
    *  Print an uint32 value as a left-justified decimal value.
    *
    * Parameters:
    *  value: A 32-bit value to be printed in ASCII characters as a decimal number
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void LCD_PrintU32Number(uint32 value) 
    {
        uint8 tmpDigit;
        char8 number[LCD_NUMBER_OF_REMAINDERS_U32 + 1u];
        uint8 digIndex = LCD_NUMBER_OF_REMAINDERS_U32;
        
        /* This API will output a decimal number as a string and that string will be 
        * filled from end to start. Set Null termination character first.
        */
        number[digIndex] = (char8) '\0';
        digIndex--;
        
        /* Load these in reverse order */
        while(value >= LCD_TEN)
        {
            /* Extract decimal digit, indexed by 'digIndex', from 'value' and
            * convert it to ASCII character.
            */
            tmpDigit = (uint8) (((uint8) (value % LCD_TEN)) + (uint8) LCD_ZERO_CHAR_ASCII);
            
            /* Temporary variable 'tmpDigit' is used to avoid Violation of MISRA rule 
            * #10.3.
            */
            number[digIndex] = (char8) tmpDigit;
            value /= LCD_TEN;
            digIndex--;
        }
        
        /* Extract last decimal digit 'digIndex', from the 'value' and convert it
        * to ASCII character.
        */
        tmpDigit = (uint8) (((uint8)(value % LCD_TEN)) + (uint8) LCD_ZERO_CHAR_ASCII);
        number[digIndex] = (char8) tmpDigit;

        /* Print out number */
        LCD_print(&number[digIndex]);
    }


void I2C_M_write_byte(uint8 addr,uint8 data)

{ 

#if CY_PSOC5
    
    /* Next functions are used for sending data to LCD from I2C master device and are taken from I2C_MASTER.c */
    
    I2C_MasterSendStart(addr, 0); /* Generates Start condition and sends slave address with read/write bit.
                                     Parameters:
                                     slaveAddress(addr):  7-bit slave address.
                                     R_nW:          Zero, send write command, non-zero send read command. Here it's set to zero. */
    
    
    I2C_MasterWriteByte(data); /* Sends one byte to a slave. A valid Start or ReStart condition must be
                                  generated before this call this function. Function do nothing if Start or
                                  Restart condition was failed before call this function. */
    
    
    I2C_MasterSendStop(); /* Generates I2C Stop condition on bus. Function do nothing if Start or Restart
                             condition was failed before call this function. */
#elif CY_PSOC4
    I2C_I2CMasterSendStart(addr, 0); /* Same as for PSOC5 devices, only for PSOC4 */
    
    I2C_I2CMasterWriteByte(data); 
    
    I2C_I2CMasterSendStop();
#endif  /* END CY_PSOC5 */
    
    return;
}
