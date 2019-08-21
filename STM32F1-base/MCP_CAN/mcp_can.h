/*
  mcp_can.h
  2012 Copyright (c) Seeed Technology Inc.  All right reserved.
  2017 Copyright (c) Cory J. Fowler  All Rights Reserved.

  Author:Loovee
  Contributor: Cory J. Fowler
  2017-09-25
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-
  1301  USA
*/
#ifndef _MCP2515_H_
#define _MCP2515_H_

#include "stm32f0xx.h"
#include "mcp_can_dfs.h"

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define MAX_CHAR_IN_MESSAGE 8

namespace MCP_CAN
{
    class Controller
    {
        private:

        int8_t   m_nExtFlg;                                                  // Identifier Type
                                                                            // Extended (29 bit) or Standard (11 bit)
        INT32U  m_nID;                                                      // CAN ID
        int8_t   m_nDlc;                                                     // Data Length Code
        int8_t   m_nDta[MAX_CHAR_IN_MESSAGE];                            	// Data array
        int8_t   m_nRtr;                                                     // Remote request flag
        int8_t   m_nfilhit;                                                  // The number of the filter that matched the message
        int8_t   MCPCS;                                                      // Chip Select pin number
        int8_t   mcpMode;                                                    // Mode to return to after configurations are performed.


    /*********************************************************************************************************
     *  mcp2515 driver function
     *********************************************************************************************************/
       // private:
       private:
        void spiInit();
        void spi_readwrite(uint8_t cmd);
        uint8_t spi_read();

        void mcp2515_reset(void);                                           // Soft Reset MCP2515

        int8_t mcp2515_readRegister(const int8_t address);                    // Read MCP2515 register

        void mcp2515_readRegisterS(const int8_t address,                     // Read MCP2515 successive registers
                                     int8_t values[],
                                   const int8_t n);

        void mcp2515_setRegister(const int8_t address,                       // Set MCP2515 register
                                 const int8_t value);

        void mcp2515_setRegisterS(const int8_t address,                      // Set MCP2515 successive registers
                                  const int8_t values[],
                                  const int8_t n);

        void mcp2515_initCANBuffers(void);

        void mcp2515_modifyRegister(const int8_t address,                    // Set specific bit(s) of a register
                                    const int8_t mask,
                                    const int8_t data);

        int8_t mcp2515_readStatus(void);                                     // Read MCP2515 Status
        int8_t mcp2515_setCANCTRL_Mode(const int8_t newmode);                 // Set mode
        int8_t mcp2515_configRate(const int8_t canSpeed,                      // Set baud rate
                                 const int8_t canClock);

        int8_t mcp2515_init(const int8_t canIDMode,                           // Initialize Controller
                           const int8_t canSpeed,
                           const int8_t canClock);

        void mcp2515_write_mf( const int8_t mcp_addr,                        // Write CAN Mask or Filter
                               const int8_t ext,
                               const INT32U id );

        void mcp2515_write_id( const int8_t mcp_addr,                        // Write CAN ID
                               const int8_t ext,
                               const INT32U id );

        void mcp2515_read_id( const int8_t mcp_addr,                         // Read CAN ID
                    int8_t* ext,
                                    INT32U* id );

        void mcp2515_write_canMsg( const int8_t buffer_sidh_addr );          // Write CAN message
        void mcp2515_read_canMsg( const int8_t buffer_sidh_addr);            // Read CAN message
        int8_t mcp2515_getNextFreeTXBuf(int8_t *txbuf_n);                     // Find empty transmit buffer

    /*********************************************************************************************************
     *  CAN operator function
     *********************************************************************************************************/

        int8_t setMsg(INT32U id, int8_t rtr, int8_t ext, int8_t len, int8_t *pData);        // Set message
        int8_t clearMsg();                                                   // Clear all message to zero
        int8_t readMsg();                                                    // Read message
        int8_t sendMsg();                                                    // Send message

    public:
        int8_t begin(int8_t idmodeset, int8_t speedset, int8_t clockset);       // Initialize controller parameters
        int8_t init_Mask(int8_t num, int8_t ext, INT32U ulData);               // Initialize Mask(s)
        int8_t init_Mask(int8_t num, INT32U ulData);                          // Initialize Mask(s)
        int8_t init_Filt(int8_t num, int8_t ext, INT32U ulData);               // Initialize Filter(s)
        int8_t init_Filt(int8_t num, INT32U ulData);                          // Initialize Filter(s)
        int8_t setMode(int8_t opMode);                                        // Set operational mode
        int8_t sendMsgBuf(INT32U id, int8_t ext, int8_t len, int8_t *buf);      // Send message to transmit buffer
        int8_t sendMsgBuf(INT32U id, int8_t len, int8_t *buf);                 // Send message to transmit buffer
        int8_t readMsgBuf(INT32U *id, int8_t *ext, int8_t *len, int8_t *buf);   // Read message from receive buffer
        int8_t readMsgBuf(INT32U *id, int8_t *len, int8_t *buf);               // Read message from receive buffer
        int8_t checkReceive(void);                                           // Check for received data
        int8_t checkError(void);                                             // Check for errors
        int8_t getError(void);                                               // Check for errors
        int8_t errorCountRX(void);                                           // Get error count
        int8_t errorCountTX(void);                                           // Get error count
        int8_t enOneShotTX(void);                                            // Enable one-shot transmission
        int8_t disOneShotTX(void);                                           // Disable one-shot transmission
        int8_t abortTX(void);                                                // Abort queued transmission(s)
        int8_t setGPO(int8_t data);                                           // Sets GPO
        int8_t getGPI(void);                                                 // Reads GPI
        void init();
        void loop();
        uint8_t MCP2515_ReadByte (uint8_t address);
        void Delay(uint32_t Delay);
    };

    extern MCP_CAN::Controller CONTROLLER;
}
#endif
/*********************************************************************************************************
 *  END FILE
 *********************************************************************************************************/
