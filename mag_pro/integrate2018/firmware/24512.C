///////////////////////////////////////////////////////////////////////////
////   Library for a 24LC512 serial EEPROM                             ////
////                                                                   ////
////   init_ext_eeprom();    Call before the other functions are used  ////
////                                                                   ////
////   write_ext_eeprom(a, d);  Write the byte d to the address a      ////
////                                                                   ////
////   d = read_ext_eeprom(a);   Read the byte d from the address a    ////
////                                                                   ////
////   The main program may define eeprom_sda                          ////
////   and eeprom_scl to override the defaults below.                  ////
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

//#ifndef EEPROM_SDA

/*#define EEPROM_SDA  PIN_C4
#define EEPROM_SCL  PIN_C5*/

#define EEPROM_SDA  PIN_C4
#define EEPROM_SCL  PIN_C3
//#endif

#use i2c(master, sda=EEPROM_SDA, scl=EEPROM_SCL)

#define ptr_start                       150
#define EEPROM_SIZE_endofkey            131071
#define EEPROM_SIZE_stofkey             115470
#define EEPROM_KEY_ST                   EEPROM_SIZE_stofkey+1
//===============================

unsigned int32 ptr_card=ptr_start;
unsigned int32 ptr_card_key=EEPROM_KEY_ST;
//===============================
void init_ext_eeprom()
{
   output_float(EEPROM_SCL);
   output_float(EEPROM_SDA);
   port_b_pullups(0xff);
}

void write_ext_eeprom(int32 address, int8 data)
{
   int1 status;
   int  command;
#if defined(24AA1025)
   if (address>65535)command=0xa8; // Check if address is located in Block 0/1
#else    
   if (address>65535)command=0xa2; // Check if address is located in Block 0/1
#endif
   else command=0xa0;

   i2c_start();
   i2c_write(command);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_write(data);

   i2c_stop();
   i2c_start();
   status=i2c_write(command);
   while(status==1)
   {
      i2c_start();
      status=i2c_write(command);
   }
   //delay_us(100);
}


BYTE read_ext_eeprom(int32 address) {
   BYTE data;
   int   command;

#if defined(24AA1025)
   if (address>65535)command=0xa8; // Check if address is located in Block 0/1
#else    
   if (address>65535)command=0xa2; // Check if address is located in Block 0/1
#endif
   else command=0xa0;

   i2c_start();
   i2c_write(command);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_start();
   i2c_write(command+1);
   data=i2c_read(0);
   i2c_stop();
   return(data);
}



int8 I2CEEPROM_read(unsigned int16 adr, unsigned int16 len, int8 *buf)
{
   unsigned int16 i;

   if((adr+len) >= EEPROM_SIZE_stofkey) return(0);
   for(i=0;i<len;i++)
   {
       buf[i]=read_ext_eeprom(adr+i);
   }
   
}

