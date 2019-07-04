// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <mcp_can.h>
#include <SPI.h>

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_33KBPS,MCP_16MHz))              // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println(" Init CAN BUS Shield again");
        delay(100);
    }
    Serial.println("CAN BUS Shield init ok!");
    for(int i=0;i<100;i++)
    {
      Msg1();
    } 
}

unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void loop()
{
   
   Msg1(); 
   
}
/*-----------------------------
Get data from ID: 40
EC	CD	13	B1	3	*/
unsigned char dattmp1[5] = {0xEC, 0xCD, 0x13, 0xB1, 0x03}; //ID : 0x40
/*
-----------------------------
Get data from ID: 4058
13	B1	1 */
unsigned char dattmp2[3] = {0x13, 0xB1, 0x01}; // ID: 0x4058	
/* -----------------------------
Get data from ID: 4040
0	A6	FF	
*/
unsigned char dattmp3[3] = {0x00, 0xA6, 0xFF}; // ID: 0x4058

/*
-----------------------------
Get data from ID: A040
0	0	0	0	4C	0	F1	0	
*/
unsigned char dattmp4[8] = {0, 0, 0, 0, 0x4C, 0, 0xF1, 0}; 
/*
-----------------------------
Get data from ID: E040
*/

/*
-----------------------------
Get data from ID: 621
0	52	0	0	0	0	0	0	
*/
unsigned char dattmp5[8] = {0, 0x52, 0, 0, 0, 0, 0, 0};
/*
-----------------------------
Get data from ID: 62C
1	12	0	0	0	0	0	0
*/
unsigned char dattmp6[8] = {0x01, 0x12, 0, 0, 0, 0, 0, 0};
/*
-----------------------------
Get data from ID: 624
1	10	0	0	0	0	0	0	
*/
unsigned char dattmp7[8] = {0x01, 0x10, 0, 0, 0, 0, 0, 0};
/*	
-----------------------------
Get data from ID: 621
0	52	0	0	0	0	0	0	
-----------------------------
Get data from ID: E040

-----------------------------
Get data from ID: 621
0	52	0	0	0	0	0	0	
-----------------------------
Get data from ID: 40
EC	CD	13	B1	3	
*/

void Msg1()
{
  
	CAN.sendMsgBuf(0x00, 0x0040, 5, dattmp1);
  
	CAN.sendMsgBuf(0x00, 0x4058, 3, dattmp2);
	
  CAN.sendMsgBuf(0x00, 0x4040, 3, dattmp3);
  
  CAN.sendMsgBuf(0x00, 0xA040, 8, dattmp4);
  
//  CAN.sendMsgBuf(0x00, 0xE040, 0, dattmp5);
//  CAN.sendMsgBuf(0x00, 0x0624, 8, dattmp7);     
  CAN.sendMsgBuf(0x00, 0x0621, 8, dattmp5);
    	
  CAN.sendMsgBuf(0x00, 0x062C, 8, dattmp6);
        
  CAN.sendMsgBuf(0x00, 0x0621, 8, dattmp5);
 
//  CAN.sendMsgBuf(0x00, 0xE040, 0, dattmp5);
      
  CAN.sendMsgBuf(0x00, 0x0621, 8, dattmp5);
  CAN.sendMsgBuf(0x00, 0x0040, 5, dattmp1);
  CAN.sendMsgBuf(0x00, 0x0624, 8, dattmp7);     
}
void Msg2()
{
  CAN.sendMsgBuf(0x00, 0x0624, 8, dattmp7);
//  CAN.sendMsgBuf(0x00, 0x0040, 5, dattmp1);
  
//  CAN.sendMsgBuf(0x00, 0x4058, 3, dattmp2);
  
//  CAN.sendMsgBuf(0x00, 0x4040, 3, dattmp3);
  
//  CAN.sendMsgBuf(0x00, 0xA040, 8, dattmp4);
  
//  CAN.sendMsgBuf(0x00, 0xE040, 0, dattmp5);
      
  CAN.sendMsgBuf(0x00, 0x0621, 8, dattmp5);
      
  CAN.sendMsgBuf(0x00, 0x062C, 8, dattmp6);
        
//  CAN.sendMsgBuf(0x00, 0x0621, 8, dattmp5);
 
//  CAN.sendMsgBuf(0x00, 0xE040, 0, dattmp5);
      
//  CAN.sendMsgBuf(0x00, 0x0621, 8, dattmp5);
//  CAN.sendMsgBuf(0x00, 0x0040, 5, dattmp1);
	
}
// END FILE
