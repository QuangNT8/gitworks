

void write_ext_eeprom(long int address, BYTE data) {
   while(!ext_eeprom_ready());
   i2c_start();
   i2c_write((0xa0|(BYTE)(address>>7))&0xfe);
   i2c_write(address);
   i2c_write(data);
   i2c_stop();
}


BYTE read_ext_eeprom(long int address) {
   BYTE data;

   //while(!ext_eeprom_ready());
   i2c_start();
   i2c_write((0xa0|(BYTE)(address>>7))&0xfe);
   i2c_write(address);
   i2c_start();
   i2c_write((0xa0|(BYTE)(address>>7))|1);
   data = i2c_read();
   i2c_stop();
   return(data);
}
