/**
  ******************************************************************************
  * @file    SmartCard_T0/src/smartcard.c  
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    13-May-2013
  * @brief   This file provides all the Smartcard firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/** @addtogroup Smartcard
  * @{
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "smartcard.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables definition and initialization ----------------------------*/
SC_ATR SC_A2R;
uint8_t SC_ATR_Table[40];
static __IO uint8_t SCData = 0;

/* F Table */
static uint32_t F_Table[16] = {372, 372, 558, 744, 1116, 1488, 1860, 0, 0, 512, 768,
                               1024, 1536, 2048, 0, 0};
/* D Table */
static uint32_t D_Table[16] = {0, 1, 2, 4, 8, 16, 32, 64, 12, 20, 0, 0, 0, 0, 0,
                               0};
/* Private function prototypes -----------------------------------------------*/
/* Transport Layer -----------------------------------------------------------*/
/*--------------APDU-----------*/
static void SC_SendData(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Response *SC_ResponseStatus);

/*------------ ATR ------------*/
static void SC_AnswerReq(SC_State *SCState, uint8_t *card, uint8_t length);  /* Ask ATR */
static uint8_t SC_decode_Answer2reset(uint8_t *card);  /* Decode ATR */

/* Physical Port Layer -------------------------------------------------------*/
static void SC_Init(void);
static void SC_DeInit(void);
static void SC_VoltageConfig(uint32_t SC_Voltage);
static uint8_t SC_Detect(void);
static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Handles all Smartcard states and serves to send and receive all
  *         communication data between Smartcard and reader.
  * @param  SCState: pointer to an SC_State enumeration that will contain the 
  *         Smartcard state.
  * @param  SC_ADPU: pointer to an SC_ADPU_Commands structure that will be initialized.  
  * @param  SC_Response: pointer to a SC_ADPU_Response structure which will be initialized.
  * @retval None
  */
void SC_Handler(SC_State *SCState, SC_ADPU_Commands *SC_ADPU, SC_ADPU_Response *SC_Response)
{ 
  uint8_t protocol = 0xFF;
  uint32_t i, j = 0;

  switch(*SCState)
  {
    case SC_POWER_ON:
      if (SC_ADPU->Header.INS == SC_GET_A2R)
      {
        /* Smartcard intialization ------------------------------------------*/
        SC_Init();

        /* Reset Data from SC buffer -----------------------------------------*/
        for (i = 0; i < 40; i++)
        {
          SC_ATR_Table[i] = 0;
        }
        
        /* Reset SC_A2R Structure --------------------------------------------*/
        SC_A2R.TS = 0;
        SC_A2R.T0 = 0;
        for (i = 0; i < MAX_PROTOCOLLEVEL; i++)
        {
            for (j = 0; j < MAX_INTERFACEBYTE; j++)
            {
              SC_A2R.T[i].InterfaceByte[j].Status = 0;
              SC_A2R.T[i].InterfaceByte[j].Value = 0;
            }
        }
        for (i = 0; i < HIST_LENGTH; i++)
        {
          SC_A2R.H[i] = 0;
        }
        
        SC_A2R.Tlength = 0;
        SC_A2R.Hlength = 0;
        
        /* Next State --------------------------------------------------------*/
        *SCState = SC_RESET_LOW;
      }
    break;

    case SC_RESET_LOW:
      if(SC_ADPU->Header.INS == SC_GET_A2R)
      {
        /* If card is detected then Power ON, Card Reset and wait for an answer) */
        if (SC_Detect())
        {
          while(((*SCState) != SC_POWER_OFF) && ((*SCState) != SC_ACTIVE))
          {
            SC_AnswerReq(SCState, &SC_ATR_Table[0], 40); /* Check for answer to reset */
          }
        }
        else
        {
          (*SCState) = SC_POWER_OFF;
        } 
      }
    break;

    case SC_ACTIVE:
      if (SC_ADPU->Header.INS == SC_GET_A2R)
      {
        protocol = SC_decode_Answer2reset(&SC_ATR_Table[0]);
        if(protocol == T0_PROTOCOL)
        {
          (*SCState) = SC_ACTIVE_ON_T0;
        }
        else if(protocol == T1_PROTOCOL)
        {
          (*SCState) = SC_ACTIVE_ON_T1;
        }
        else  
        {
          (*SCState) = SC_POWER_OFF; 
        }
      }
    break;

    case SC_ACTIVE_ON_T0:
      SC_SendData(SC_ADPU, SC_Response);
    break;
    
    case SC_ACTIVE_ON_T1:
      SC_SendData(SC_ADPU, SC_Response);
    break;

    case SC_POWER_OFF:
      SC_DeInit(); /* Disable Smartcard interface */
    break;

    default: (*SCState) = SC_POWER_OFF;
  }
}

/**
  * @brief  Enables or disables the power to the Smartcard.
  * @param  NewState: new state of the Smartcard power supply. 
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SC_PowerCmd(FunctionalState NewState)
{
  if(NewState != DISABLE)
  {
    GPIO_ResetBits(SC_CMDVCC_GPIO_PORT, SC_CMDVCC_PIN);
  }
  else
  {
    GPIO_SetBits(SC_CMDVCC_GPIO_PORT, SC_CMDVCC_PIN);
  } 
}

/**
  * @brief  Sets or clears the Smartcard reset pin.
  * @param  ResetState: this parameter specifies the state of the Smartcard 
  *         reset pin. BitVal must be one of the BitAction enum values:
  *                 @arg Bit_RESET: to clear the port pin.
  *                 @arg Bit_SET: to set the port pin.
  * @retval None
  */
void SC_Reset(BitAction ResetState)
{
  GPIO_WriteBit(SC_RESET_GPIO_PORT, SC_RESET_PIN, ResetState);
}

/**
  * @brief  Resends the byte that failed to be received (by the Smartcard) correctly.
  * @param  None
  * @retval None
  */
void SC_ParityErrorHandler(void)
{
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  } 
}

/**
  * @brief  Configures the IO speed (BaudRate) communication.
  * @param  None
  * @retval None
  */
void SC_PTSConfig(void)
{
  RCC_ClocksTypeDef RCC_ClocksStatus;
  uint32_t workingbaudrate = 0, apbclock = 0;
  uint8_t locData = 0, PPSConfirmStatus = 1;
  USART_InitTypeDef USART_InitStructure;


 /* Reconfigure the USART Baud Rate -------------------------------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  apbclock = RCC_ClocksStatus.USART1CLK_Frequency;
  apbclock /= ((SC_USART->GTPR & (uint16_t)0x00FF) * 2);
 
  if((SC_A2R.T0 & (uint8_t)0x10) == 0x10)
  {
    if(SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value != 0x11)
    {
      /* PPSS identifies the PPS request or responce and is equal to 0xFF */
      SCData = 0xFF;
      /* Send PPSS */
      USART_SendData(SC_USART, SCData);
      while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
      {
      }

      /* PPS0 indicates by the bits b5, b6, b7 equal to 1 the presence of the optional
      bytes PPSI1, PPS2, PPS3 respectively */
      SCData = 0x10;
      /* Send PPS0 */
      USART_SendData(SC_USART, SCData);
      while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
      {
      }
      /* PPS1 allows the interface device to propose value of F and D to the card */
      
      SCData = SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value; 
      /* Send PPS1 */
      USART_SendData(SC_USART, SCData);
      while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
      {
      }

      /* PCK check character */ 
      SCData = (uint8_t)0xFF^(uint8_t)0x10^(uint8_t)SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value; 
      /* Send PCK */
      USART_SendData(SC_USART, SCData);
      while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
      {
      }
      
      /* PPSS response = PPSS request = 0xFF*/
      if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
      {
        if(locData != 0xFF)
        {
           /*PPSS exchange unsuccessful */
           PPSConfirmStatus = 0x00;
        }
      }
      /*PPS0 response */
      if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
      {
        if(locData != 0x10)
        {
           /*PPS0 exchange unsuccessful */
           PPSConfirmStatus = 0x00;
        }
      }
      
      /*PPS1 response */
      if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
      {
        if(locData != SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value)
        {
           /*PPS1 exchange unsuccessful */
           PPSConfirmStatus = 0x00;
        }
      }
      
      if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
      {
        if(locData != ((uint8_t)0xFF^(uint8_t)0x10^(uint8_t)SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value))
        {
           /*PCK exchange unsuccessful */
           PPSConfirmStatus = 0x00;
        }
      }
      else
      {
        /*PPS exchange unsuccessful */
        PPSConfirmStatus = 0x00;
      }
      
      /* PPS exchange successful */
      if(PPSConfirmStatus == 0x01)
      {
        workingbaudrate = apbclock * D_Table[(SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value & (uint8_t)0x0F)];
        workingbaudrate /= F_Table[((SC_A2R.T[0].InterfaceByte[SC_INTERFACEBYTE_TA].Value >> 4) & (uint8_t)0x0F)];

        USART_InitStructure.USART_BaudRate = workingbaudrate;
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(SC_USART, &USART_InitStructure);
       
      }
    }
  }
}

/**
  * @brief  Configures GPIO hardware resources used for Samrtcard.
  * @param  None
  * @retval None
  */
void SC_IOConfig(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(SC_OFF_GPIO_CLK | SC_CMDVCC_GPIO_CLK | 
                        SC_3_5V_GPIO_CLK | SC_RESET_GPIO_CLK, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure Smartcard CMDVCC pin */
  GPIO_InitStructure.GPIO_Pin = SC_CMDVCC_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SC_CMDVCC_GPIO_PORT, &GPIO_InitStructure);

  /* Configure Smartcard Reset pin */
  GPIO_InitStructure.GPIO_Pin = SC_RESET_PIN;
  GPIO_Init(SC_RESET_GPIO_PORT, &GPIO_InitStructure);

  /* Configure Smartcard 3/5V pin */
  GPIO_InitStructure.GPIO_Pin = SC_3_5V_PIN;
  GPIO_Init(SC_3_5V_GPIO_PORT, &GPIO_InitStructure);

  /* Select 5V */ 
  SC_VoltageConfig(SC_VOLTAGE_5V);

  /* Disable CMDVCC */
  SC_PowerCmd(DISABLE);

  /* Set RSTIN HIGH */  
  SC_Reset(Bit_SET);
    						                                
  /* Configure Smartcard OFF pin */
  GPIO_InitStructure.GPIO_Pin = SC_OFF_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SC_OFF_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure EXTI line connected to Smartcard OFF Pin */
  SYSCFG_EXTILineConfig(SC_OFF_EXTI_PORT_SOURCE, SC_OFF_EXTI_PIN_SOURCE);

  EXTI_ClearITPendingBit(SC_OFF_EXTI_LINE);
  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_Line = SC_OFF_EXTI_LINE;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Clear the SC_OFF_EXTI_IRQn Pending Bit */
  NVIC_ClearPendingIRQ(SC_OFF_EXTI_IRQn);
  NVIC_InitStructure.NVIC_IRQChannel = SC_OFF_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);    
}

/**
  * @brief  Manages the Smartcard transport layer: send APDU commands and receives
  *   the APDU responce.
  * @param  SC_ADPU: pointer to a SC_ADPU_Commands structure which will be initialized.  
  * @param  SC_Response: pointer to a SC_ADPU_Response structure which will be initialized.
  * @retval None
  */
static void SC_SendData(SC_ADPU_Commands *SC_ADPU, SC_ADPU_Response *SC_ResponseStatus)
{
  uint32_t i = 0;
  uint8_t locData = 0;

  /* Reset responce buffer ---------------------------------------------------*/
  for(i = 0; i < LC_MAX; i++)
  {
    SC_ResponseStatus->Data[i] = 0;
  }
  
  SC_ResponseStatus->SW1 = 0;
  SC_ResponseStatus->SW2 = 0;

  /* Enable the DMA Receive (Set DMAR bit only) to enable interrupt generation
     in case of a framing error FE */  
  USART_DMACmd(SC_USART, USART_DMAReq_Rx, ENABLE);

  /* Send header -------------------------------------------------------------*/
  SCData = SC_ADPU->Header.CLA;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  }  
  
  SCData = SC_ADPU->Header.INS;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  }
   
  SCData = SC_ADPU->Header.P1;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  } 
  
  SCData = SC_ADPU->Header.P2;
  USART_SendData(SC_USART, SCData);
  while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
  {
  }   

  /* Send body length to/from SC ---------------------------------------------*/
  if(SC_ADPU->Body.LC)
  {
    SCData = SC_ADPU->Body.LC;
    USART_SendData(SC_USART, SCData);
    while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
    {
    }     
  }
  else if(SC_ADPU->Body.LE)
  { 
    SCData = SC_ADPU->Body.LE;
    USART_SendData(SC_USART, SCData);
    while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
    {
    }     
  }
  /* Flush the SC_USART DR */
  (void)USART_ReceiveData(SC_USART);

  /* --------------------------------------------------------
    Wait Procedure byte from card:
    1 - ACK
    2 - NULL
    3 - SW1; SW2
   -------------------------------------------------------- */

  if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
  {
    if(((locData & (uint8_t)0xF0) == 0x60) || ((locData & (uint8_t)0xF0) == 0x90))
    {
      /* SW1 received */
      SC_ResponseStatus->SW1 = locData;

      if((USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT)) == SUCCESS)
      {
        /* SW2 received */
        SC_ResponseStatus->SW2 = locData;
      }
    }
    else if (((locData & (uint8_t)0xFE) == (((uint8_t)~(SC_ADPU->Header.INS)) & \
             (uint8_t)0xFE))||((locData & (uint8_t)0xFE) == (SC_ADPU->Header.INS & (uint8_t)0xFE)))
    {
      SC_ResponseStatus->Data[0] = locData;/* ACK received */
    }
  }

  /* If no status bytes received ---------------------------------------------*/
  if(SC_ResponseStatus->SW1 == 0x00)
  {
    /* Send body data to SC--------------------------------------------------*/
    if (SC_ADPU->Body.LC)
    {
      for(i = 0; i < SC_ADPU->Body.LC; i++)
      {
        SCData = SC_ADPU->Body.Data[i];
        
        USART_SendData(SC_USART, SCData);
        while(USART_GetFlagStatus(SC_USART, USART_FLAG_TC) == RESET)
        {
        } 
      }
      /* Flush the SC_USART DR */
      (void)USART_ReceiveData(SC_USART);
      /* Disable the DMA Receive (Reset DMAR bit only) */  
      USART_DMACmd(SC_USART, USART_DMAReq_Rx, DISABLE);
    }

    /* Or receive body data from SC ------------------------------------------*/
    else if (SC_ADPU->Body.LE)
    {
      for(i = 0; i < SC_ADPU->Body.LE; i++)
      {
        if(USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT) == SUCCESS)
        {
          SC_ResponseStatus->Data[i] = locData;
        }
      }
    }
    /* Wait SW1 --------------------------------------------------------------*/
    i = 0;
    while(i < 10)
    {
      if(USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT) == SUCCESS)
      {
        SC_ResponseStatus->SW1 = locData;
        i = 11;
      }
      else
      {
        i++;
      }
    }
    /* Wait SW2 ------------------------------------------------------------*/   
    i = 0;
    while(i < 10)
    {
      if(USART_ByteReceive(&locData, SC_RECEIVE_TIMEOUT) == SUCCESS)
      {
        SC_ResponseStatus->SW2 = locData;
        i = 11;
      }
      else
      {
        i++;
      }
    }
  }
}

/**
  * @brief  Requests the reset answer from card.
  * @param  SCState: pointer to an SC_State enumeration that will contain the Smartcard state.
  * @param  card: pointer to a buffer which will contain the card ATR.
  * @param  length: maximum ATR length
  * @retval None
  */
static void SC_AnswerReq(SC_State *SCState, uint8_t *card, uint8_t length)
{
  uint8_t Data = 0;
  uint32_t i = 0;

  switch(*SCState)
  {
    case SC_RESET_LOW:
      /* Check responce with reset low ---------------------------------------*/
      for (i = 0; i < length; i++)
      {
        if((USART_ByteReceive(&Data, SC_RECEIVE_TIMEOUT)) == SUCCESS)
        {
          card[i] = Data;
        }
      }
      if(card[0])
      {
        (*SCState) = SC_ACTIVE;
        SC_Reset(Bit_SET);
      }
      else
      {
        (*SCState) = SC_RESET_HIGH;
      }
    break;

    case SC_RESET_HIGH:
      /* Check responce with reset high --------------------------------------*/
      SC_Reset(Bit_SET); /* Reset High */
  
      while(length--)
      {
        if((USART_ByteReceive(&Data, SC_RECEIVE_TIMEOUT)) == SUCCESS)
        {
          *card++ = Data; /* Receive data for timeout = SC_RECEIVE_TIMEOUT */
        }       
      }
      if(card[0])
      {
        (*SCState) = SC_ACTIVE;
      }
      else
      {
        (*SCState) = SC_POWER_OFF;
      }
    break;

    case SC_ACTIVE:
    break;
    
    case SC_POWER_OFF:
      /* Close Connection if no answer received ------------------------------*/
      SC_Reset(Bit_SET); /* Reset high - a bit is used as level shifter from 3.3 to 5 V */
      SC_PowerCmd(DISABLE);
    break;

    default:
      (*SCState) = SC_RESET_LOW;
  }
}

/**
  * @brief  Decodes the Answer to reset received from card.
  * @param  card: pointer to the buffer containing the card ATR.
  * @retval None
  */
static uint8_t SC_decode_Answer2reset(uint8_t *card)
{
  uint32_t i = 0, flag = 0, protocol = 0;
  uint8_t index = 0, level = 0;
  
/******************************TS/T0 Decode************************************/
  SC_A2R.TS = card[index++];  /* Initial character */
  SC_A2R.T0 = card[index++];  /* Format character */

/*************************Historical Table Length Decode***********************/
  SC_A2R.Hlength = SC_A2R.T0 & (uint8_t)0x0F;
  
/******************************Protocol Level(1) Decode************************/  
  /* Check TD(1) if present */
  if ((SC_A2R.T0 & (uint8_t)0x80) == 0x80)
  {
    flag = 1;
  }
  
  /* Each bits in the T0 high nibble(b8 to b5) equal to 1 indicates the presence
     of a further interface byte */
  for (i = 0; i < 4; i++)
  {
    if (((SC_A2R.T0 & (uint8_t)0xF0) >> (4 + i)) & (uint8_t)0x1)
    {
      SC_A2R.T[level].InterfaceByte[i].Status = 1;
      SC_A2R.T[level].InterfaceByte[i].Value = card[index++];
      SC_A2R.Tlength++;
    }
  }
  
/*****************************T Decode*****************************************/ 
  if (SC_A2R.T[level].InterfaceByte[3].Status == 1)
  {
    /* Only the protocol(parameter T) present in TD(1) is detected
       if two or more values of parameter T are present in TD(1), TD(2)..., so the 
       firmware should be updated to support them */
    protocol = SC_A2R.T[level].InterfaceByte[SC_INTERFACEBYTE_TD].Value  & (uint8_t)0x0F;
  }
  else
  {
    protocol = 0;
  }
  
  /* Protocol Level Increment */

/******************************Protocol Level(n>1) Decode**********************/ 
  while (flag)
  {
    if ((SC_A2R.T[level].InterfaceByte[SC_INTERFACEBYTE_TD].Value & (uint8_t)0x80) == 0x80)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    }
    /* Each bits in the high nibble(b8 to b5) for the TD(i) equal to 1 indicates
      the presence of a further interface byte */
    for (i = 0; i < 4; i++)
    {
      if (((SC_A2R.T[level].InterfaceByte[SC_INTERFACEBYTE_TD].Value & (uint8_t)0xF0) >> (4 + i)) & (uint8_t)0x1)
      {
        SC_A2R.T[level+1].InterfaceByte[i].Status = 1;
        SC_A2R.T[level+1].InterfaceByte[i].Value = card[index++];
        SC_A2R.Tlength++;
      }
    }
    level++; 
  }
  
  for (i = 0; i < SC_A2R.Hlength; i++)
  {
    SC_A2R.H[i] = card[i + 2 + SC_A2R.Tlength];
  }
/*************************************TCK Decode*******************************/
  SC_A2R.TCK = card[SC_A2R.Hlength + 2 + SC_A2R.Tlength];
  
  return (uint8_t)protocol;
}

/**
  * @brief  Initializes all peripheral used for Smartcard interface.
  * @param  None
  * @retval None
  */
static void SC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef USART_ClockInitStructure;  
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Enable GPIO clocks */
  RCC_AHBPeriphClockCmd(SC_USART_TX_GPIO_CLK | SC_USART_CK_GPIO_CLK, ENABLE);
                         
  /* Enable SC_USART clock */
  SC_USART_APBPERIPHCLOCK(SC_USART_CLK, ENABLE);

  /* Connect PXx to SC_USART_TX */
  GPIO_PinAFConfig(SC_USART_TX_GPIO_PORT, SC_USART_TX_SOURCE, SC_USART_TX_AF);
  
  /* Connect PXx to SC_USART_CK */
  GPIO_PinAFConfig(SC_USART_CK_GPIO_PORT, SC_USART_CK_SOURCE, SC_USART_CK_AF); 

  /* Configure USART Tx and Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = SC_USART_CK_PIN;
  GPIO_Init(SC_USART_CK_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure USART Tx pin as alternate function open-drain */
  GPIO_InitStructure.GPIO_Pin = SC_USART_TX_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(SC_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Enable SC_USART IRQ */
  NVIC_InitStructure.NVIC_IRQChannel = SC_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);
    
/* SC_USART configuration ----------------------------------------------------*/
  /* SC_USART configured as follow:
        - Word Length = 9 Bits
        - 1.5 Stop Bit
        - Even parity
        - BaudRate = 8064 baud
        - Hardware flow control disabled (RTS and CTS signals)
        - Tx and Rx enabled
        - USART Clock enabled
  */

  /* USART Clock set to 4 MHz (PCLK2 (48 MHz) / 12) */
  USART_SetPrescaler(SC_USART, 0x06);
  
  /* USART Guard Time set to 16 Bit */
  USART_SetGuardTime(SC_USART, 16);
  
  USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
  USART_ClockInit(SC_USART, &USART_ClockInitStructure);
  /*Configure the initial baudrate*/
  USART_InitStructure.USART_BaudRate = 10753;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_Init(SC_USART, &USART_InitStructure); 
  
  /* Enable the SC_USART Parity Error Interrupt */
  USART_ITConfig(SC_USART, USART_IT_PE, ENABLE);

  /* Enable the SC_USART Framing Error Interrupt */
  USART_ITConfig(SC_USART, USART_IT_ERR, ENABLE);
  
  /* Enable the NACK Transmission */
  USART_SmartCardNACKCmd(SC_USART, ENABLE);

  /* Enable the Smartcard Interface */
  USART_SmartCardCmd(SC_USART, ENABLE);
  
  /*Configure the auto retry*/
  USART_SetAutoRetryCount(SC_USART, 0x06);
  
  /* Enable SC_USART */
  USART_Cmd(SC_USART, ENABLE);
  
  /* Set RSTIN HIGH */  
  SC_Reset(Bit_SET);
}

/**
  * @brief  Deinitializes all ressources used by the Smartcard interface.
  * @param  None
  * @retval None
  */
static void SC_DeInit(void)
{
  /* Disable CMDVCC */
  SC_PowerCmd(DISABLE);

  /* Deinitializes the SC_USART */
  USART_DeInit(SC_USART);
                         
  /* Disable SC_USART clock */
  SC_USART_APBPERIPHCLOCK(SC_USART_CLK, DISABLE);
}

/**
  * @brief  Configures the card power voltage.
  * @param  SC_Voltage: specifies the card power voltage.
  *         This parameter can be one of the following values:
  *              @arg SC_VOLTAGE_5V: 5V cards.
  *              @arg SC_VOLTAGE_3V: 3V cards.
  * @retval None
  */
static void SC_VoltageConfig(uint32_t SC_Voltage)
{
  if(SC_Voltage == SC_VOLTAGE_5V)
  {
    /* Select Smartcard 5V */  
    GPIO_SetBits(SC_3_5V_GPIO_PORT, SC_3_5V_PIN);
  }
  else
  {
    /* Select Smartcard 3V */      
    GPIO_ResetBits(SC_3_5V_GPIO_PORT, SC_3_5V_PIN);
  } 
}

/**
  * @brief  Detects whether the Smartcard is present or not.
  * @param  None. 
  * @retval 0 - Smartcard inserted
  *         1 - Smartcard not inserted
  */
static uint8_t SC_Detect(void)
{
  return GPIO_ReadInputDataBit(SC_OFF_GPIO_PORT, SC_OFF_PIN);
}

/**
  * @brief  Receives a new data while the time out not elapsed.
  * @param  None
  * @retval An ErrorStatus enumuration value:
  *          - SUCCESS: New data has been received
  *          - ERROR: time out was elapsed and no further data is received
  */
static ErrorStatus USART_ByteReceive(uint8_t *Data, uint32_t TimeOut)
{
  uint32_t Counter = 0;

  while((USART_GetFlagStatus(SC_USART, USART_FLAG_RXNE) == RESET) && (Counter != TimeOut))
  {
    Counter++;
  }

  if(Counter != TimeOut)
  {
    *Data = (uint8_t)USART_ReceiveData(SC_USART);
    return SUCCESS;    
  }
  else 
  {
    return ERROR;
  }
}


/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
