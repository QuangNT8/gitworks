#define lcd_type 2           // 0=5x7, 1=5x10, 2=2 lines
#define lcd_line_two 0x40    // LCD RAM address for the second line

BYTE const LCD_INIT_STRING[4] = {0x20 | (lcd_type << 2), 0xc, 1, 6};
                             // These bytes need to be sent to the LCD
                             // to start it up.


                             // The following are used for setting
                             // the I/O port direction register.

/*my LCD*/
#define  bg_night          PIN_B2
#define  lcd_rw            PIN_B3
#define  lcd_rs            PIN_B4
#define  lcd_enable        PIN_B5
#define  D4                PIN_C0
#define  D5                PIN_C1
#define  D6                PIN_C2
#define  D7                PIN_C3
#define  turnon_bgn        output_high(bg_night)
#define  turnoff_bgn       output_low(bg_night)
/*customer*/
/*
//#define  bg_night          PIN_D0
//#define  lcd_rw            PIN_D1
#define  lcd_rs            PIN_D1
#define  lcd_enable        PIN_D0
#define  D4                PIN_A3
#define  D5                PIN_A2
#define  D6                PIN_A1
#define  D7                PIN_A0
*/
//===============================================

/*BYTE lcd_read_byte() {
      BYTE low,high,bufer;
      //set_tris_lcd(LCD_READ);
      output_high(lcd_rw);
      delay_cycles(1);
      output_high(lcd_enable);
      delay_cycles(1);
      high = (input(D7)|high)<<1;
      high = (input(D6)|high)<<1;
      high = (input(D5)|high)<<1;
      high = (input(D4)|high);
      output_low(lcd_enable);
      delay_cycles(1);
      output_high(lcd_enable);
      delay_us(1);
      low = (input(D7)|low)<<1;
      low = (input(D6)|low)<<1;
      low = (input(D5)|low)<<1;
      low = (input(D4)|low);
      output_low(lcd_enable);
      //set_tris_lcd(LCD_WRITE);
      return( (high<<4) | low);
}
*/
const unsigned char malcd[10]={'0','1','2','3','4','5','6','7','8','9'};
void lcd_send_nibble( BYTE n ) {
      int1 bufer;
      bufer=n>>0;
      output_bit(D4,bufer);
      bufer=n>>1;
      output_bit(D5,bufer);
       bufer=n>>2;
      output_bit(D6,bufer);
       bufer=n>>3;
      output_bit(D7,bufer);
      delay_us(10);
      //output_d(n<<4);
      delay_cycles(1);
      output_high(lcd_enable);
      delay_us(2);
      output_low(lcd_enable);
}


void lcd_send_byte( BYTE address, BYTE n ) {

      output_low(lcd_rs);
      delay_us(30);
      output_bit(lcd_rs,address);
      delay_us(1);
      output_low(lcd_rw);
      delay_cycles(1);
      output_low(lcd_enable);
      lcd_send_nibble(n >> 4);
      lcd_send_nibble(n & 0xf);
}


void lcd_init() {
    BYTE i;
    //set_tris_lcd(LCD_WRITE);
    output_low(lcd_rs);
    output_low(lcd_rw);
    output_low(lcd_enable);
    delay_ms(15);
    for(i=1;i<=3;++i) {
       lcd_send_nibble(3);
       delay_ms(5);
    }
    lcd_send_nibble(2);
    for(i=0;i<=3;++i)
       lcd_send_byte(0,LCD_INIT_STRING[i]);
}


void lcd_gotoxy( BYTE x, BYTE y) {
   BYTE address;

   if(y!=1)
     address=lcd_line_two;
   else
     address=0;
   address+=x-1;
   lcd_send_byte(0,0x80|address);
}

void lcd_putc( char c) {
   switch (c) {
     case '\f'   : lcd_send_byte(0,1);
                   delay_ms(2);
                                           break;
     case '\n'   : lcd_gotoxy(1,2);        break;
     case '\b'   : lcd_send_byte(0,0x10);  break;
     default     : lcd_send_byte(1,c);     break;
   }
}

/*char lcd_getc( BYTE x, BYTE y) {
   char value;

    lcd_gotoxy(x,y);
    while ( bit_test(lcd_read_byte(),7) ); // wait until busy flag is low
    output_high(lcd_rs);
    value = lcd_read_byte();
    output_low(lcd_rs);
    return(value);
}*/
