/*
 * lcdput.c - emulate putchar for LCD display on DE2 board
 * Written by F Lundevall 2007
 * Modified by F Lundevall 2009
 * Copyright abandoned. This file is in the public domain.
 */
#define TIMER_100U ((volatile int *) 0x8a0 )
#define LCD_CTRL  ( (volatile int *) 0x808 )
#define LCD_ROWSIZE  (16)
#define LCD_FUNCCODE (0x38)

static void lcd_write_instr( int icode )
{
  while( (*LCD_CTRL & 0x80) == 0x80 ); /* busy, wait */
  *LCD_CTRL = (icode & 0xff); /* write */
}

static void lcd_write_data( int c )
{
  volatile int * p = LCD_CTRL;
  while( (*LCD_CTRL & 0x80) == 0x80 ); /* busy, wait */
  p[1] = c & 0xff;
}
/*
 * lcdinit - initialize LCD module
 */
void lcdinit()
{
  volatile int * p = TIMER_100U;
  int r;

  p[1] = 6; /* continuous mode, no interrupt,
               plus redundant START bit */
  while( (p[0] & 1) == 0 ); /* wait for timeout */
  p[0] = 17; /* reset timeout flag */
  
  /* initialization sequence, step 1:
   * wait for more than 15 ms after power-up */
  for( r = 0; r <= 15000 /* us */; r += 100 /* us per tick */)
  { while( (p[0] & 1) == 0 ); p[0] = 17; };
  *LCD_CTRL = LCD_FUNCCODE; /* Function Set */

  /* initialization sequence, step 2:
   * wait for more than 4.1 ms, then Function Set again */
  for( r = 0; r <= 4100 /* us */; r += 100 /* us per tick */)
  { while( (p[0] & 1) == 0 ); p[0] = 17; };
  *LCD_CTRL = LCD_FUNCCODE; /* Function Set (again)*/
  
  /* initialization sequence, step 3:
   * wait for more than 100 us, then Function Set again */
  for( r = 0; r <= 100 /* us */; r += 100 /* us per tick */)
  { while( (p[0] & 1) == 0 ); p[0] = 17; };
  *LCD_CTRL = LCD_FUNCCODE; /* Function Set (again)*/
  
  /* initialization sequence, step 4, 5, 6, 7:
   * Function Set, display off, clear, entry mode */
  while( (p[0] & 1) == 0 ); p[0] = 17; /* wait just in case */
  lcd_write_instr( LCD_FUNCCODE ); /* Function Set (again!!!)*/
  lcd_write_instr( 0x80 ); /* display off */
  lcd_write_instr( 0x01 ); /* display off */
  lcd_write_instr( 0x06 ); /* cursor direction & shift */
}

/*
 * lcdput - new version 2009.
 * 
 * New features and bugfixes in this version:
 * 1) Checks for cursor moving beyond right edge of screen.
 * 2) Scrolls display when doing newline from lower row.
 * 
 * Control codes:
 * 0x0a == '\n', line feed, move to the next line.
 *     For this LCD driver, line feed has the side effect
 *     that the cursor is reset to the beginning of the line.
 * 0x0d == '\r', carriage return, move to beginning of same line.
 * 0x0c == 12, form feed, move to upper left corner of display.
 *
 * Static (persistent) local variables:
 * onlowrow = 0 for upper (initial value), 1 for lower.
 * currentpos = 1 for left edge (initial value), 16 at right edge.
 * lowrow[] = contents of row 1, used for scrolling, origin 1.
 * 
 * Temporary local variables:
 * i = loop counter.
 */
void lcdput( int c )
{
  static int onlowrow = 0;
  static int currentpos = 1;
  static char lowrow[17]; /* Need 17 elements since we use origin 1. */
  
  int i;
  
  c = (c & 0xff);

  if( 13 == c ) /* Carriage Return, move to left edge on same row. */
  {
    if( onlowrow ) lcd_write_instr( 0xc0 ); /* Cursor to lower left corner. */
    else           lcd_write_instr( 0x80 ); /* Cursor to upper left corner. */
  }
  else if( 12 == c ) /* Form-feed, clear display. */
  {
    onlowrow = 0;
    currentpos = 1;
    lcd_write_instr( 1 );
    lcd_write_instr( 0x80 ); /* Cursor to upper left corner. */
  }
  else if( 10 == c ) /* Newline. */
  {
    if( onlowrow ) /* We are on lower row already, scroll. */
    {
      lcd_write_instr( 1 ); /* Clear display. */
      lcd_write_instr( 0x80 ); /* Cursor to upper left corner. */
      for( i = 1; i <= 16; i += 1 )
        if( i < currentpos ) lcd_write_data( lowrow[ i ] );
        else lcd_write_data( ' ' );
    }
    lcd_write_instr( 0xc0 ); /* Cursor to lower left corner. */
    onlowrow = 1;
    currentpos = 1;
  }
  else if( onlowrow ) /* Send normal character to lower row. */
  {
    lowrow[ currentpos ] = c;
    lcd_write_data( c );
    currentpos += 1;
    if( currentpos > 16 ) /* Uh-oh, past the edge. Now scroll. */
    {
      lcd_write_instr( 1 ); /* Clear display. */
      lcd_write_instr( 0x80 ); /* Cursor to upper left corner. */
      for( i = 1; i <= 16; i += 1 )
        if( i < currentpos ) lcd_write_data( lowrow[ i ] );
        else lcd_write_data( ' ' );
      lcd_write_instr( 0xc0 ); /* Cursor to lower left corner. */
      currentpos = 1;
    }
  }
  else /* Send normal character to upper row. */
  {
    lcd_write_data( c );
    currentpos += 1;
    if( currentpos > 16 ) /* Uh-oh, past the edge. */
    {    
      lcd_write_instr( 0xc0 ); /* Cursor to lower left corner. */
      onlowrow = 1;
      currentpos = 1;
    }
  }
}
