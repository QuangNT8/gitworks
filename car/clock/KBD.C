///////////////////////////////////////////////////////////////////////////
////                             KBDD.C                                ////
////                  Generic keypad scan driver                       ////
////                                                                   ////
////  kbd_init()   Must be called before any other function.           ////
////                                                                   ////
////  c = kbd_getc(c)  Will return a key value if pressed or /0 if not ////
////                   This function should be called frequently so as ////
////                   not to miss a key press.                        ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2003 Custom Computer Services           ////
//// This source code may only be used by licensed users of the CCS C  ////
//// compiler.  This source code may only be distributed to other      ////
//// licensed users of the CCS C compiler.  No other use, reproduction ////
//// or distribution is permitted without written permission.          ////
//// Derivative programs created using this software in object code    ////
//// form are not restricted in any way.                               ////
///////////////////////////////////////////////////////////////////////////

////////////////// The following defines the keypad layout on port D

// Un-comment the following define to use port B
// #define use_portb_kbd TRUE

// Make sure the port used has pull-up resistors (or the LCD) on
// the column pins


/*#if defined(__PCH__)
#if defined use_portb_kbd
   #byte kbd = 0xF81                   // This puts the entire structure
#else
   #byte kbd = 0xF83                   // This puts the entire structure
#endif
#else
#if defined use_portb_kbd
   #byte kbdc = 6                  // on to port B (at address 6)
   #byte kbdr = 5 #byte kbdc = 6                  // on to port B (at address 6)
#else
   #byte kbd = 8                 // on to port D (at address 8)
   #byte kbdr = 5 #endif*/

//#byte kbdr = 5
//#byte kbdc = 6 
//#define set_tris_kbd(x) set_tris_b(x)
/*
#define  ROW0      PIN_A0
#define  ROW1      PIN_A1
#define  ROW2      PIN_A2
#define  ROW3      PIN_A3
#define  ROW4      PIN_A4

#define  COL0      PIN_B5
#define  COL1      PIN_B4
#define  COL2      PIN_B3
#define  COL3      PIN_B2
#define  COL4      PIN_B1

*/
//Keypad connection:   (for example column 0 is B2)
//                Bx:

#define  COL0      PIN_A0
#define  COL1      PIN_A1
#define  COL2      PIN_A2
#define  COL3      PIN_A3
#define  COL4      PIN_A4

#define  ROW0      PIN_B5
#define  ROW1      PIN_B4
#define  ROW2      PIN_B3
#define  ROW3      PIN_B2
#define  ROW4      PIN_B1
#define  turnoff_bgn_af30s  enable_interrupts(INT_RTCC);
//#define ALL_ROWS (ROW0|ROW1|ROW2|ROW3|ROW3)
//#define ALL_COLS (COL0|COL1|COL2|COL3|COL4)

// Keypad layout:
char const KEYS[5][5] = {{'<','R','>','D','L'},
                         {'1','3','6','U','2'},
                         {'4','6','9','O','5'},
                         {'7','9','#','G','8'},
                         {'*','#','K','M','0'}
                         };

#define KBD_DEBOUNCE_FACTOR  30    // Set this number to apx n/333 where
                                  // n is the number of times you expect
                                  // to call kbd_getc each second

unsigned int32 count_timer0=0;

void kbd_init() {
}

char kbd_getc() {
   static BYTE kbd_call_count;
   static int1 kbd_down;
   static char last_key;
   static BYTE col;

   BYTE kchar;
   BYTE row;
   //kchar='t';//'\0';
   if(++kbd_call_count>KBD_DEBOUNCE_FACTOR) {    
       switch (col) {
         case 0   : 
                    output_low(COL0);
                    output_high(COL1);
                    output_high(COL2);
                    output_high(COL3);
                    output_high(COL4);
                    break;
         case 1   : 
                    output_low(COL1);
                    output_high(COL0);
                    output_high(COL2);
                    output_high(COL3);
                    output_high(COL4);
                    break;
         case 2   : 
                    output_low(COL2);
                    output_high(COL1);
                    output_high(COL0);
                    output_high(COL3);
                    output_high(COL4);
                    break;
         case 3   : 
                    output_low(COL3);
                    output_high(COL1);
                    output_high(COL2);
                    output_high(COL0);
                    output_high(COL4);
                    break;
         case 4   : 
                    output_low(COL4);
                    output_high(COL1);
                    output_high(COL2);
                    output_high(COL3);
                    output_high(COL0);
                    break;
       }

       if(kbd_down) 
       {
         if(input(ROW0)&input(ROW1)&input(ROW2)&input(ROW3)&input(ROW4))
         {
            count_timer0=0;
           turnon_bgn;
           //turnoff_bgn_af30s;
           //&count_time_lcd=0;
           
           kchar=last_key;
           //lcd_gotoxy(0,1);
           //lcd_putc(kchar);
           kbd_down=FALSE;
           last_key='\0';
         }
       } else 
         {
             if(!(input(ROW0)&input(ROW1)&input(ROW2)&input(ROW3)&input(ROW4)))
             {
                if(input(ROW0)==0)
                  row=0;
                else if(input(ROW1)==0)
                  row=1;
                else if(input(ROW2)==0)
                  row=2;
                else if(input(ROW3)==0)
                  row=3;
                else if(input(ROW4)==0) 
                  row=4;
                last_key =KEYS[row][col];
                kbd_down = TRUE;
                //lcd_gotoxy(0,1);
                //lcd_putc(last_key);
             } else 
               {
                  ++col;
                  if(col==5) col=0;
               }
         }
      kbd_call_count=0;
   }
  return(kchar);
}
