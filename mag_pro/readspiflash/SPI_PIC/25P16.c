///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////  Library for a MicroChip 93C56 EEPROM configured for a x8 org     ////
////                                                                   ////
////  #use spi() is used to configure the SPI pins on your PIC.  If    ////
////  you use hardware SPI pins, the library will automatically use    ////
////  the SPI peripheral.  If you use pins that are not SPI pins,      ////
////  the library will fall back to bit banging thos GPIO pins.        ////
////                                                                   ////
////                                                                   ////
////  Required configuration:                                          ////
////  ---------------------------------------------------------        ////
////     EEPROM_SELECT - Chip select, active low                       ////
////     EEPROM_CLK - SPI clock                                        ////
////     EEPROM_DI - SPI MOSI                                          ////
////     EEPROM_DO - SPI MISO                                          ////
////                                                                   ////
////                                                                   ////
////  API:                                                             ////
////  ---------------------------------------------------------        ////
////   init_ext_eeprom();    Call before the other functions are used  ////
////                                                                   ////
////   write_ext_eeprom(a, d);  Write the byte d to the address a      ////
////                                                                   ////
////   d = read_ext_eeprom(a);   Read the byte d from the address a    ////
////                                                                   ////
////   The main program may define eeprom_select, eeprom_di, eeprom_do ////
////   and eeprom_clk to override the defaults below.                  ////
////                                                                   ////
////                                                                   ////
////  Example files:                                                   ////
////  ---------------------------------------------------------        ////
////     ex_spi.c - Demo of using this library to read/write.          ////
////                                                                   ////
////                                                                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

#define EEPROM_SELECT PIN_C2
#define EEPROM_CLK    PIN_D6
#define EEPROM_DI     PIN_A5  //mosi
#define EEPROM_DO     PIN_C0  //miso

//opcodes
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2
#define CMD_WRITE_ENABLE                    (0x06)
#define CMD_WRITE_DISABLE                   (0x04)
#define CMD_READ_IDENTIFICATION             (0x9F)
#define CMD_READ_STATUS_REGISTER            (0x05)
#define CMD_WRITE_STATUS_REGISTER           (0x01)
#define CMD_READ_DATA_BYTES                 (0x03)
#define CMD_PAGE_PROGRAM                    (0x02)
#define CMD_SECTOR_ERASE                    (0xD8)
#define CMD_BULK_ERASE                      (0xC7)
#define CMD_DEEP_POWER_DOWN                 (0xB9)
#define CMD_RELEASE_FROM_DEEP_POWER_DOWN    (0xAB)

#use spi(master, di=EEPROM_DI, do=EEPROM_DO, clk=EEPROM_CLK, mode=0,BITS=8,MSB_FIRST,BAUD=100000,stream=STREAM_SPI_25P16)

#define SPI_XFER_25P16(x)   spi_xfer(STREAM_SPI_25P16, x)

#define EEPROM_ADDRESS unsigned int8
#define EEPROM_SIZE    256

typedef struct {
  int8 manufacturer;
  int8 memory_type;
  int8 memory_capacity;
  int8 cfd_length;
  int8 cfd_content[16];
} m25p16_identification_t;


void init_ext_eeprom(void) 
{
   output_high(EEPROM_SELECT);
   output_low(EEPROM_DI);
   output_low(EEPROM_CLK);
   output_float(EEPROM_DO);
}


void write_ext_eeprom(EEPROM_ADDRESS address, BYTE data) 
{
   output_high(EEPROM_SELECT);
   
   SPI_XFER_25P16(0xa);
   SPI_XFER_25P16(address);
   SPI_XFER_25P16(data);
   
   output_low(EEPROM_SELECT);
   
   delay_ms(11);
}


int8 read_ext_eeprom(int32 address) 
{
   int8 data;

   output_low(EEPROM_SELECT);
   
   SPI_XFER_25P16(5);
   //SPI_XFER_25P16((int8)(address>>16));
   //SPI_XFER_25P16((int8)(address>>8));
   //SPI_XFER_25P16(0);
   //SPI_XFER_25P16(0);
   //SPI_XFER_25P16((int8)(address));
   data=SPI_XFER_25P16(0);
   
   output_high(EEPROM_SELECT);

   return(data);
}

void m25p16_read_data_bytes(int32 addr, int8 *buf, int32 siz)
{
  int32 i;
  output_low(EEPROM_SELECT);
  SPI_XFER_25P16(CMD_READ_DATA_BYTES);
  SPI_XFER_25P16(addr >> 16);
  SPI_XFER_25P16(addr >>  8);
  SPI_XFER_25P16(addr >>  0);
  for (i = 0; i < siz; i++) {
    buf[i] = SPI_XFER_25P16(0);
  }
  output_high(EEPROM_SELECT);
}

void m25p16_read_status_register(int8 *sreg)
{
   output_low(EEPROM_SELECT);
   SPI_XFER_25P16(CMD_READ_STATUS_REGISTER);
   *sreg = SPI_XFER_25P16(0);
   output_high(EEPROM_SELECT);
}

void m25p16_read_identification(m25p16_identification_t *p)
{
  int i;
  output_low(EEPROM_SELECT);
  delay_ms(10);
  SPI_XFER_25P16(CMD_READ_IDENTIFICATION);
  p->manufacturer = SPI_XFER_25P16(0);
  p->memory_type = SPI_XFER_25P16(0);
  p->memory_capacity = SPI_XFER_25P16(0);
  p->cfd_length = SPI_XFER_25P16(0);
  for (i = 0; i < p->cfd_length; i++) {
    p->cfd_content[i] = SPI_XFER_25P16(0);
  }
  delay_ms(10);
  output_high(EEPROM_SELECT);
}