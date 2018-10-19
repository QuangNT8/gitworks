/**
  ******************************************************************************
  * @file    Smartcard_T1/src/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    13-May-2013
  * @brief   Main program body.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "t1_hal.h"
#include "t1_protocol.h"
#include "t1_protocol_param.h"
#include "stm320518_eval.h"
#include <stdio.h>

/** @addtogroup SmartCard_T1
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Modifiable parameters */
#define SAD           0x0  /* Source address: reader (allowed values 0 -> 7) */
#define DAD           0x0  /* Destination address: card (allowed values 0 -> 7) */
#define IFSD_VALUE    254  /* Max length of INF field Supported by the reader */
#define SC_FILE_SIZE  0x100   /* File size */
#define SC_FILE_ID    0x0001  /* File identifier */
#define SC_CLASS      0x00

/* Constant parameters */
#define INS_SELECT_FILE    0xA4 /* Select file instruction */
#define INS_READ_FILE      0xB0 /* Read file instruction */
#define INS_WRITE_FILE     0xD6 /* Write file instruction */
#define TRAILER_LENGTH     2    /* Trailer lenght (SW1 and SW2: 2 bytes) */

/* Private macro -------------------------------------------------------------*/
#define NAD    (((DAD&0x7)<<4) | (SAD&0x7)) /* Node Address byte */

/* Private variables ---------------------------------------------------------*/
ATR_TypeDef atr; /* Answer To Reset structure */
t1_TypeDef T1;   /* T=1 protocol structure */
__IO int32_t ResponseStatus = 0; /* Communication Response status */
uint8_t C_APDU[300];  /* APDU Command buffer */
uint8_t R_APDU[300];  /* APDU Response buffer */
uint8_t ATR_buf[ATR_MAX_SIZE]; /* Answer To Reset buffer */
uint32_t CardClkFreq = 0;  /* The Smartcard clock frequency in Hz */
uint32_t etu_usec = 0; /* Elementary Time Unit in microsecond */
uint32_t SC_baud = 0;  /* SmartCard baudrate */
int8_t protocol= -1;   /* Default protocol (initialized to -1) */
uint32_t apdu_index;   /* Index of the APDU buffer */
uint8_t atr_status;    /* The status of the ATR response */
__IO uint32_t TimingDelay = 0; 
__IO uint32_t CardInserted = 0;

/* Private function prototypes -----------------------------------------------*/
static void Delay(uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */   
  
  
  /* ---Smart Card init--- */
  
  /* Initialize the hardware for Smartcard application */
  SC_HardwareInit();
  
  /* ---LED init--- */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  
  /* ---Smart Card not inserted--- */
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOff(LED4);
  
  /* Loop while no Smartcard is detected */  
  while(CardInserted == 0)
  {
  }

  
  /* ---Smart Card inserted--- */
  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  
  /* Insert delay of 100ms for signal stabilization */
  Delay(100);
    
  /* Configure the USART for SmartCard application and get the selected frequency */
  CardClkFreq = SC_USARTConfig(&etu_usec,&SC_baud);
  
  /* Enable CMDVCC */
  SC_PowerCmd(ENABLE);
  
  /* Reset the card */
  SC_Reset(Bit_RESET);
  
  
  /************************* Answer To Reset (ATR) ******************************/ 
  
  /* ---ATR request...--- */
  
  /* Get the answer to reset (ATR) frame from the card */    
  SC_GetATR(ATR_buf, etu_usec, 17); 
  
  /* Decode the ATR frame */
  atr_status = ATR_Decode(&atr, &ATR_buf[0], ATR_MAX_SIZE);
  
  /* If the ATR is malformed */
  if(atr_status != 0)
  {
    /* ---ATR ERROR--- */
    while (1)
    {
      /* The user can reset the card at this stage */
    }
  }
  else
  {
    
    /* ---ATR OK--- */
    /* ---ATR received: ATR_buf, atr.length--- */
    
  }
  
  /* ---SmartCard Clk: CardClkFreq--- */
  /* ---SmartCard baudrate: SC_baud--- */
  
  
  /* Get the default protocol */
  ATR_GetDefaultProtocol(&atr, &protocol);
  
  /* If the protocol used by the card is T=1, start the demo */
  if(protocol == ATR_PROTOCOL_TYPE_T1)
  {
    
    /********************** T=1 Protocol initialization ****************************/ 
    
    /* Init the protocol structure */
    T1_Protocol_Init(&T1,CardClkFreq);
    
    /* Set F and D parameters and get the new etu value in micro-seconds */
    etu_usec = Set_F_D_parameters(&atr,CardClkFreq);
    
    /* Set the etu to be used by the protocol */
    (void)T1_SetParameter(&T1, T1_PROTOCOL_ETU, etu_usec);
    
    /* Set the convention of the protocol */
    Set_Convention(&atr,&T1);
    
    /* Set and configure the extra guardtime value */
    Set_EGT(&atr); 
    
    /* Set the IFSC (card IFS) value */
    Set_IFSC(&atr, &T1);
    
    /* Set the CWT and BWT values */
    Set_CWT_BWT(&atr, &T1);      
    
    /* Set the EDC type (LRC or CRC) */
    Set_EDC(&atr, &T1); 
    
    /*************************** Start T1 Protocol ********************************/ 
    
    /*------- Send IFSD request --------------------------------------------------*/     
    
    /* Negotiate IFSD: we indicate to the card a new IFSD that the reader can support */
    ResponseStatus = T1_Negotiate_IFSD(&T1, NAD, IFSD_VALUE);
    
    /* If the IFSD request communication has failed */
    if(ResponseStatus<0)
    {
      /* ---IFSD communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    
    
    /*------- Send APDU: Select File Command -------------------------------------*/     
    
    /* Send Select File Command */  
    C_APDU[0] = SC_CLASS;                    /* CLA */
    C_APDU[1] = INS_SELECT_FILE;             /* INS: Select File */
    C_APDU[2] = 0x00;                        /* P1 */
    C_APDU[3] = 0x00;                        /* P2 */
    C_APDU[4] = 0x02;                        /* Lc */
    C_APDU[5] = (uint8_t)(SC_FILE_ID>>8);    /* Data 1 */
    C_APDU[6] = (uint8_t)(SC_FILE_ID&0xFF);  /* Data 2 */
    
    /* ---Sending APDU:  C_APDU, 7--- */
    
    /* Send/Receive APDU command/response: Select File having ID = SC_FILE_ID */
    ResponseStatus = T1_APDU(&T1, NAD, C_APDU, 7, R_APDU, TRAILER_LENGTH);
    
    /* If the APDU communication has failed */
    if(ResponseStatus<0)
    {
      /* ---APDU communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    else
    {
      /* ---APDU response: R_APDU, ResponseStatus--- */    
    }
    
    
    /*------- Send APDU: Read File Command ---------------------------------------*/     
    
    /* Select file */
    C_APDU[0] = SC_CLASS;       /* CLA */
    C_APDU[1] = INS_READ_FILE;  /* INS: Read File */
    C_APDU[2] = 0x00;           /* P1 */
    C_APDU[3] = 0x00;           /* P2 */
    C_APDU[4] = 0x00;           /* Lc: read 256 bytes */
    
    /* ---Sending APDU: C_APDU, 5--- */ 
    
    /* Send/Receive APDU command/response: Read File (256 bytes) */
    ResponseStatus = T1_APDU(&T1, NAD, C_APDU, 5, R_APDU, 256+TRAILER_LENGTH);
    
    /* If the APDU communication has failed */
    if(ResponseStatus<0)
    {
      /* ---APDU communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    else
    { 
      /* ---APDU response: R_APDU, ResponseStatus--- */ 
    }
    
    
    /*------- Send APDU: Write File Command --------------------------------------*/      
    
    /* Select file */
    C_APDU[0] = SC_CLASS;        /* CLA */
    C_APDU[1] = INS_WRITE_FILE;  /* INS: Write File */
    C_APDU[2] = 0x00;            /* P1 */
    C_APDU[3] = 0x00;            /* P2 */
    C_APDU[4] = 0xFF;            /* Lc */
    
    apdu_index = 5;
    
    while(apdu_index<261)
    {
      /* initialize the APDU buffer (data field) */
      C_APDU[apdu_index] = apdu_index-5;
      apdu_index++;
    }
    
    /* ---Sending APDU--- */
    
    /* Send/Receive APDU command/response: Write File 256 bytes */
    ResponseStatus = T1_APDU(&T1, NAD, C_APDU, 261, R_APDU, TRAILER_LENGTH);
    
    /* If the APDU communication has failed */
    if(ResponseStatus<0)
    {   
      /* ---APDU communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    else
    {     
      /* ---APDU response: R_APDU, ResponseStatus--- */     
    }
    
    
    /*------- Send APDU: Read File Command ---------------------------------------*/     
    
    /* Select file */
    C_APDU[0] = SC_CLASS;       /* CLA */
    C_APDU[1] = INS_READ_FILE;  /* INS: Read File */
    C_APDU[2] = 0x00;           /* P1 */
    C_APDU[3] = 0x00;           /* P2 */
    C_APDU[4] = 0x00;           /* Lc */
    
    /* ---Sending APDU:  C_APDU, 5--- */ 
    
    /* Send/Receive APDU command/response: Read File (256 bytes) */
    ResponseStatus = T1_APDU(&T1, NAD, C_APDU, 5, R_APDU, 256+TRAILER_LENGTH);
    
    /* If the APDU communication has failed */
    if(ResponseStatus<0)
    {
      /* ---APDU communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    else
    {   
      /* ---APDU response: R_APDU, ResponseStatus--- */ 
    }
    
    
    
    /*------- Send APDU: Write File Command --------------------------------------*/        
    
    /* Select file */
    C_APDU[0] = SC_CLASS;        /* CLA */
    C_APDU[1] = INS_WRITE_FILE;  /* INS: Write File */
    C_APDU[2] = 0x00;            /* P1 */
    C_APDU[3] = 0x00;            /* P2 */
    C_APDU[4] = 0xFF;            /* Lc */
    
    apdu_index = 5;
    
    while(apdu_index<261)
    {
      /* Get and invesre the file received from the card */
      C_APDU[apdu_index] = ~(R_APDU[apdu_index-5]);
      apdu_index++;
    }
    
    /* ---Sending APDU: ", C_APDU, 261--- */
    
    /* Send/Receive APDU command/response: Write File (256 bytes) */
    ResponseStatus =  T1_APDU(&T1, NAD, C_APDU, 261, R_APDU, TRAILER_LENGTH);
    
    /* If the APDU communication has failed */
    if(ResponseStatus<0)
    {  
      /* ---APDU communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    else
    {
      /* ---APDU response: R_APDU, ResponseStatus--- */
    }
    
    /*------- Send APDU: Read File Command -------------------------------------*/     
    
    /* Select file */
    C_APDU[0] = SC_CLASS;       /* CLA */
    C_APDU[1] = INS_READ_FILE;  /* INS: Read File */
    C_APDU[2] = 0x00;           /* P1 */
    C_APDU[3] = 0x00;           /* P2 */
    C_APDU[4] = 0x00;           /* Lc */
    
    /* ---Sending APDU: C_APDU, 5--- */
    
    /* Send/Receive APDU command/response: Read File (256 bytes) */
    ResponseStatus =  T1_APDU(&T1, NAD, C_APDU, 5, R_APDU, 256+TRAILER_LENGTH);
    
    /* If the APDU communication has failed */
    if(ResponseStatus<0)
    {
      /* ---APDU communication error--- */
      while (1)
      {
        /* The user can reset the card at this stage */
      }
    }
    else
    { 
      /* ---APDU response:  R_APDU, ResponseStatus--- */ 
      /* ---All operations were executed successfully --- */
      STM_EVAL_LEDOn(LED1);
      STM_EVAL_LEDOn(LED2);
      STM_EVAL_LEDOn(LED3);
      STM_EVAL_LEDOn(LED4);
    }     
  }
  else
  {
    /* ---SmartCard is not compatible with T=1 protocol--- */
    STM_EVAL_LEDOff(LED1);
    STM_EVAL_LEDOn(LED2);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);
  }

  /* ----*-*-*- DEMO END -*-*-*---- */   
  CardInserted = 0;
  while (1)
  {
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length (time base 10 ms).
  * @retval None
  */
static void Delay(uint32_t nCount)
{
  TimingDelay = nCount;
 
  while(TimingDelay != 0)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
