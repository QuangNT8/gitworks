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

#define EEPROM_ADDRESS long int
//#define EEPROM_SIZE   65535
#define EEPROM_SIZE_key    65535
#define EEPROM_SIZE        43590
#define EEPROM_KEY_ST      EEPROM_SIZE+1
//===============================
#define ptr_start                150

//===============================
void init_ext_eeprom()
{
   output_float(EEPROM_SCL);
   output_float(EEPROM_SDA);
   port_b_pullups(0xff);
}

void write_ext_eeprom(long int address, BYTE data)
{
   short int status;
   i2c_start();
   i2c_write(0xa0);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
   i2c_start();
   status=i2c_write(0xa0);
   while(status==1)
   {
      i2c_start();
      status=i2c_write(0xa0);
   }
   i2c_stop();
}

BYTE read_ext_eeprom(long int address) {
   BYTE data;
   i2c_start();
   i2c_write(0xa0);
   i2c_write(address>>8);
   i2c_write(address);
   i2c_start();
   i2c_write(0xa1);
   data=i2c_read(0);
   i2c_stop();
   return(data);
}

int8 I2CEEPROM_read(unsigned int16 adr, unsigned int16 len, int8 *buf)
{
   unsigned int16 i;

   if((adr+len) >= EEPROM_SIZE) return(0);
   for(i=0;i<len;i++)
   {
       buf[i]=read_ext_eeprom(adr+i);
   }
   
}

