
#define RS485_ID  0xf0                                  // The device's RS485 address or ID
#define RS485_RX_ENABLE       output_bit( PIN_B1, 0);    // Controls RE pin.  Should keep low.
#define RS485_RX_DISABLE      output_bit( PIN_B1, 1);
#define BUS_BUSY              0x0f
#define BUS_FREE              0x00
#define BYTE_START            0x01
#define BYTE1                 1
#define BYTE2                 2
#define BYTE3                 3
#define BYTE4                 4
#define ID                    5
#define dat1                  6
#define dat2                  7
#define dat3                  8
#define BYTE_STOP             0x02
//#use rs232(uart1, baud=2400)
//#use rs232(baud=2400,parity=N,xmit=PIN_C4,rcv=PIN_C5,bits=8)
//#define RS485_wait_time 20             // Wait time in milliseconds
#define RS485_RX_BUFFER_SIZE  10
//byte0 la byte_start
//byte1 la byte thong tin chung1
//byte2 la byte thong tin chung2
//byte3 la byte thong tin chung3
//byte4 la byte thong tin chung4
//byte5 la byte ID cua thiet bi nhan
//byte6 la byte Dieu khien 1
//byte7 la byte dieu khien 2
//byte8 la byte dieu khien 3
//byte9 la byte_stop
int8 count_byte = 0;
int rs485_buffer[RS485_RX_BUFFER_SIZE];
int rs485_buffer_TX[RS485_RX_BUFFER_SIZE];
//======================================
void rs485_init() 
{
   //enable_interrupts(INT_RDA);
   //enable_interrupts(GLOBAL);
   RS485_RX_ENABLE;
}
//========================================
unsigned int8 check_packet()
{
   unsigned int8 temp; 
      if((rs485_buffer[0] == BYTE_START)&& ((rs485_buffer[9] == BYTE_STOP)))  // kiem tra goi tin xem co noi ko
      {
         temp = 0xff;
      }
      else temp = 0;
      return temp;
}
//========================================
int8 temp_ni;
void rs485_add_to_temp(int8 b) {
   // Store the byte
   rs485_buffer[temp_ni] = b;

   // Make the index cyclic
   if(++temp_ni >= RS485_RX_BUFFER_SIZE)
   {
      temp_ni = 0;
   }
}
//========================================================
void rs485_send_message() 
{
   int8 i;
   RS485_RX_DISABLE;
   delay_us(1000);
   for(i=0;i<RS485_RX_BUFFER_SIZE;i++)
   {
      putc(rs485_buffer_TX[i]);
      //delay_us(1);
   }
   delay_us(1000);
   RS485_RX_ENABLE;
   
}
//========================================================
void rs485_wait_for_bus(int1 clrwdt)
{
      if(clrwdt)
         restart_wdt();
}
//=====================================================
void sendto485(int8 num_packets,unsigned char addr, unsigned char dat)
{
   int8 i;
   for(i=0;i<num_packets;i++)
   {
         rs485_buffer_TX[0]= 0x01;
         rs485_buffer_TX[1]= BUS_BUSY;
         rs485_buffer_TX[2]=  0;
         rs485_buffer_TX[3]=  0;
         rs485_buffer_TX[4]=  0;
         rs485_buffer_TX[5] = addr;   //dia chi thiet bi
         rs485_buffer_TX[6] =  dat;   //lenh bat
         rs485_buffer_TX[7]=  0;
         rs485_buffer_TX[8]=  0;
         rs485_buffer_TX[9]=  0x02;
         rs485_send_message();
         //delay_ms(1);
   }
}
//====================================================
void send_status(unsigned char addr, unsigned char dat)
{
   //if(rs485_buffer[1]==0)
   //{
         rs485_buffer_TX[0]= 0x01;
         rs485_buffer_TX[1]= BUS_BUSY;// bao chiem duong truyen
         rs485_buffer_TX[2]=0;
         rs485_buffer_TX[3]=0;
         rs485_buffer_TX[4]=0;
         rs485_buffer_TX[5] = addr;   //dia chi may bom
         rs485_buffer_TX[6] =  dat;   //lenh bat
         rs485_buffer_TX[7]=0;
         rs485_buffer_TX[8]=0;
         rs485_buffer_TX[9]=0x02;
         rs485_send_message();
   //}
}
//=====================================================
