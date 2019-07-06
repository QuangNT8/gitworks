// CAN Receive Example
//

#include <mcp_can.h>
#include <SPI.h>

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 2                              // Set INT to pin 2
MCP_CAN CAN0(10);                               // Set CS to pin 10


void setup()
{
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_33K3BPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
	//received();
	Msg1();
}

void send()
{
	
}

void received()
{
	if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
	{
		CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

		if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
		  sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
		else
		  sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);

		Serial.print(msgString);

		if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
		  sprintf(msgString, " REMOTE REQUEST FRAME");
		  Serial.print(msgString);
		} else {
		  for(byte i = 0; i<len; i++){
			sprintf(msgString, " 0x%.2X", rxBuf[i]);
			Serial.print(msgString);
		  }
		}
			
		Serial.println();
	}
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

*/
unsigned char dattmp5[8] = {0, 0, 0, 0, 0, 0, 0, 0};
/*
-----------------------------
Get data from ID: 621
0	52	0	0	0	0	0	0	
*/
unsigned char dattmp6[8] = {0, 0x52, 0, 0, 0, 0, 0, 0};
/*
-----------------------------
Get data from ID: 62C
1	12	0	0	0	0	0	0
*/
unsigned char dattmp7[8] = {0x01, 0x12, 0, 0, 0, 0, 0, 0};
/* 
-----------------------------
Get data from ID: 4060
0	
-----------------------------
 */
unsigned char dattmp8[1] = {0};
/* 
-----------------------------
Get data from ID: 40
0	0	80	0	0	0	80	0	
-----------------------------
 */
unsigned char dattmp9[8] = {0, 0, 0x80, 0, 0, 0, 0x80, 0}; 
/* 
-----------------------------
Get data from ID: C060
0	
-----------------------------
*/
unsigned char dattmp10[1] = {0};
/*  
-----------------------------
Get data from ID: 40
80	
-----------------------------
*/
unsigned char dattmp11[1] = {0x80};

/*  
-----------------------------
Get data from ID: 6060
0	
-----------------------------	
*/
unsigned char dattmp12[1] = {0};
/*
*/

/*  
Extended ID: 0x103D6060  DLC: 1  Data: 0x00
-----------------------------	
*/
unsigned char dattmp13[1] = {0};
/*
*/
void Msg1()
{
    CAN0.sendMsgBuf(0x102C0040, 5, dattmp1);
	  
    CAN0.sendMsgBuf(0x10304058, 3, dattmp2);
    
    CAN0.sendMsgBuf(0x10644040, 3, dattmp3);
    
    CAN0.sendMsgBuf(0x102CA040, 8, dattmp4);
    
    CAN0.sendMsgBuf(0x13FFE040, 0, dattmp5);
    
    CAN0.sendMsgBuf(0x621, 8, dattmp6);
    
    CAN0.sendMsgBuf(0x62C, 8, dattmp7);
        
    CAN0.sendMsgBuf(0x10244060, 1, dattmp8);
    
    CAN0.sendMsgBuf(0x10210040, 8, dattmp9);
    
    CAN0.sendMsgBuf(0x103BC060, 1, dattmp10);
    
    CAN0.sendMsgBuf(0x0C630040, 1, dattmp11);
    
    CAN0.sendMsgBuf(0x103DA060, 1, dattmp12);
    
    CAN0.sendMsgBuf(0x103D6060, 1, dattmp13);	
}
void Msg2()
{
    CAN0.sendMsgBuf(0x0040, 0, 5, dattmp1);

    CAN0.sendMsgBuf(0x4058, 0, 3, dattmp2);
     
    CAN0.sendMsgBuf(0x4040, 0, 3, dattmp3);
     
    CAN0.sendMsgBuf(0xA040, 0, 8, dattmp4);
     
    CAN0.sendMsgBuf(0xE040, 0, 0, dattmp5);
     
    CAN0.sendMsgBuf(0x621, 0, 8, dattmp6);
     
    CAN0.sendMsgBuf(0x62C, 0, 8, dattmp7);
        
    CAN0.sendMsgBuf(0x4060, 10, 1, dattmp8);
     
    CAN0.sendMsgBuf(0x0040, 0, 8, dattmp9);
     
    CAN0.sendMsgBuf(0xC060, 0, 1, dattmp10);
     
    CAN0.sendMsgBuf(0x0040, 0, 1, dattmp11);
     
    CAN0.sendMsgBuf(0xA060, 0, 1, dattmp12);
  
    CAN0.sendMsgBuf(0x6060, 0, 1, dattmp13);  
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
