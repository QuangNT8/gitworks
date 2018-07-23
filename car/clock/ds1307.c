
#define DS1307_SEC      0
#define DS1307_MIN      1   
#define DS1307_HR       2
#define DS1307_DAY      3
#define DS1307_DATE     4
#define DS1307_MONTH    5
#define DS1307_YR       6
#define DS1307_CONTROL  7

//===============================
unsigned int8 day,year=14,h=0,min=0,sec=0;
//===============================

BYTE read_DS1307(BYTE address)
{
   BYTE result;

   i2c_start();
   i2c_write(0xD0);
   i2c_write(address);
   i2c_start();
   i2c_write(0xD1);
   result = i2c_read(0);
   i2c_stop();
   
   return(result);
}

void write_DS1307(BYTE address, BYTE data)
{
   i2c_start();
   delay_us(10);
   i2c_write(0xD0);
   delay_us(10);
   i2c_write(address);
   delay_us(10);
   i2c_write(data);
   i2c_stop();
   delay_us(10);
}

void rtc_init()
{
   unsigned int8 data;
   
   data = read_DS1307(DS1307_SEC);
   
   data &= 0x7F;
   
   i2c_start();
   i2c_write(0xD0);
   i2c_write(DS1307_SEC);
   i2c_write(data);
   i2c_start();
   i2c_write(0xD0);
   i2c_write(DS1307_CONTROL);
   i2c_write(0x80);
   i2c_stop();
}

unsigned int8 get_bcd(BYTE data)
{
   unsigned int8 nibh;
   unsigned int8 nibl;

   nibh=data/10;
   nibl=data-(nibh*10);

   return((nibh<<4)|nibl);
}

unsigned int8 rm_bcd(BYTE data)
{
   unsigned int8 i;

   i=data;
   data=(i>>4)*10;
   data=data+(i<<4>>4);

   return data;
}

void rtc_set_datetime(BYTE day, BYTE mth, BYTE year, BYTE dow, BYTE hr, BYTE min)
{
   /*i2c_start();
   i2c_write(DS1307_SEC);
   //i2c_write(0x00);
   i2c_write(get_bcd(min));
   i2c_write(get_bcd(hr));
   i2c_write(get_bcd(dow));
   i2c_write(get_bcd(day));
   //i2c_write(0x01);
   i2c_write(get_bcd(mth));
   i2c_write(get_bcd(year));
   i2c_stop();*/
   write_DS1307(DS1307_SEC,0x00);
   write_DS1307(DS1307_MIN,get_bcd(min));
   write_DS1307(DS1307_HR,get_bcd(hr));
   write_DS1307(DS1307_DAY,get_bcd(dow));
   write_DS1307(DS1307_DATE,get_bcd(day));
   write_DS1307(DS1307_MONTH,get_bcd(mth));
   write_DS1307(DS1307_YR,get_bcd(year));
   write_DS1307(DS1307_CONTROL,0);   
}

void rtc_get_date(BYTE &date, BYTE &mth, BYTE &year, BYTE &dow)
{
   date = rm_bcd(read_DS1307(DS1307_DATE));
   mth = rm_bcd(read_DS1307(DS1307_MONTH));
   year = rm_bcd(read_DS1307(DS1307_YR));
   dow = rm_bcd(read_DS1307(DS1307_DAY));
}

void rtc_get_time(BYTE &hr, BYTE &min, BYTE &sec)
{
   hr = rm_bcd(read_DS1307(DS1307_HR));
   min = rm_bcd(read_DS1307(DS1307_MIN));
   sec = rm_bcd(read_DS1307(DS1307_SEC));
}

void rtc_write_nvr(BYTE address, BYTE data)
{
   write_DS1307(address, data);
}

BYTE rtc_read_nvr(BYTE address)
{
   return(read_DS1307(address));
}
//====================================
void mysettime()
{
   rtc_set_datetime(9,9,14,0,1,0);
}
//====================================
