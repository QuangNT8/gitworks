
#include "stm32f103xb.h"


#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_dma.h"
#include "stm32f1xx_hal_dma_ex.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_can.h"

#include"Stm32_GPIO/gpio.h"
#include "uart/controller.h"
#include "command.h"

#include "CAN/can.h"


uint32_t checkvalue;
UART_HandleTypeDef huart1;
CAN_HandleTypeDef hcan;

CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

uint32_t CAN_SendTxMessage(uint32_t DTLC, uint32_t addr, uint8_t *aData)
{
    uint32_t transmitmailbox;
    uint32_t tsr = READ_REG(CAN1->TSR);

    /* Check that all the Tx mailboxes are not full */
    if (((tsr & CAN_TSR_TME0) != 0U) ||
        ((tsr & CAN_TSR_TME1) != 0U) ||
        ((tsr & CAN_TSR_TME2) != 0U))
    {
        /* Select an empty transmit mailbox */
        transmitmailbox = (tsr & CAN_TSR_CODE) >> CAN_TSR_CODE_Pos;

        CAN1->sTxMailBox[transmitmailbox].TIR = ((addr << CAN_TI0R_STID_Pos));

        /* Set up the DLC */
        CAN1->sTxMailBox[transmitmailbox].TDTR = DTLC;

//        SET_BIT(CAN1->sTxMailBox[transmitmailbox].TDTR, CAN_TDT0R_TGT);

          /* Set up the data field */
          WRITE_REG(CAN1->sTxMailBox[transmitmailbox].TDHR,
                    ((uint32_t)aData[7] << CAN_TDH0R_DATA7_Pos) |
                    ((uint32_t)aData[6] << CAN_TDH0R_DATA6_Pos) |
                    ((uint32_t)aData[5] << CAN_TDH0R_DATA5_Pos) |
                    ((uint32_t)aData[4] << CAN_TDH0R_DATA4_Pos));
          WRITE_REG(CAN1->sTxMailBox[transmitmailbox].TDLR,
                    ((uint32_t)aData[3] << CAN_TDL0R_DATA3_Pos) |
                    ((uint32_t)aData[2] << CAN_TDL0R_DATA2_Pos) |
                    ((uint32_t)aData[1] << CAN_TDL0R_DATA1_Pos) |
                    ((uint32_t)aData[0] << CAN_TDL0R_DATA0_Pos));

          /* Request transmission */
          SET_BIT(CAN1->sTxMailBox[transmitmailbox].TIR, CAN_TI0R_TXRQ);
    }
}


void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /** NONJTRST: Full SWJ (JTAG-DP + SW-DP) but without NJTRST
  */
  __HAL_AFIO_REMAP_SWJ_NONJTRST();

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
//uint8_t datatest[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};

uint8_t CAN_Transmit(uint32_t addr, uint32_t data_size, uint8_t * tab_data)
{
//    if(data_size > 8 || sizeof(tab_data) / sizeof(tab_data[0]) <= data_size)
//	return 0;

    TxHeader.StdId = addr;
    TxHeader.DLC = data_size;

    for(int i = 0; i < data_size; i++)
    {
        TxData[i] = tab_data[i];
    }
//    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    CAN_SendTxMessage(data_size, addr, tab_data);
    return 1;
}

#define CAN_Prescaler   80
#define CAN_MODE_NORMAL             (0x00000000U)                              /*!< Normal mode   */
#define CAN_MODE_LOOPBACK           ((uint32_t)CAN_BTR_LBKM)                   /*!< Loopback mode */
#define CAN_MODE_SILENT             ((uint32_t)CAN_BTR_SILM)                   /*!< Silent mode   */
#define CAN_MODE_SILENT_LOOPBACK    ((uint32_t)(CAN_BTR_LBKM | CAN_BTR_SILM))  /*!< Loopback combined with silent mode */

/** @defgroup CAN_time_quantum_in_bit_segment_1 CAN Time Quantum in Bit Segment 1
  * @{
  */
#define CAN_BS1_1TQ                 (0x00000000U)                                                /*!< 1 time quantum  */
#define CAN_BS1_2TQ                 ((uint32_t)CAN_BTR_TS1_0)                                    /*!< 2 time quantum  */
#define CAN_BS1_3TQ                 ((uint32_t)CAN_BTR_TS1_1)                                    /*!< 3 time quantum  */
#define CAN_BS1_4TQ                 ((uint32_t)(CAN_BTR_TS1_1 | CAN_BTR_TS1_0))                  /*!< 4 time quantum  */
#define CAN_BS1_5TQ                 ((uint32_t)CAN_BTR_TS1_2)                                    /*!< 5 time quantum  */
#define CAN_BS1_6TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_0))                  /*!< 6 time quantum  */
#define CAN_BS1_7TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1))                  /*!< 7 time quantum  */
#define CAN_BS1_8TQ                 ((uint32_t)(CAN_BTR_TS1_2 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 8 time quantum  */
#define CAN_BS1_9TQ                 ((uint32_t)CAN_BTR_TS1_3)                                    /*!< 9 time quantum  */
#define CAN_BS1_10TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_0))                  /*!< 10 time quantum */
#define CAN_BS1_11TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1))                  /*!< 11 time quantum */
#define CAN_BS1_12TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_1 | CAN_BTR_TS1_0))  /*!< 12 time quantum */
#define CAN_BS1_13TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2))                  /*!< 13 time quantum */
#define CAN_BS1_14TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_0))  /*!< 14 time quantum */
#define CAN_BS1_15TQ                ((uint32_t)(CAN_BTR_TS1_3 | CAN_BTR_TS1_2 | CAN_BTR_TS1_1))  /*!< 15 time quantum */
#define CAN_BS1_16TQ                ((uint32_t)CAN_BTR_TS1) /*!< 16 time quantum */
/**
  * @}
  */

/** @defgroup CAN_time_quantum_in_bit_segment_2 CAN Time Quantum in Bit Segment 2
  * @{
  */
#define CAN_BS2_1TQ                 (0x00000000U)                                /*!< 1 time quantum */
#define CAN_BS2_2TQ                 ((uint32_t)CAN_BTR_TS2_0)                    /*!< 2 time quantum */
#define CAN_BS2_3TQ                 ((uint32_t)CAN_BTR_TS2_1)                    /*!< 3 time quantum */
#define CAN_BS2_4TQ                 ((uint32_t)(CAN_BTR_TS2_1 | CAN_BTR_TS2_0))  /*!< 4 time quantum */
#define CAN_BS2_5TQ                 ((uint32_t)CAN_BTR_TS2_2)                    /*!< 5 time quantum */
#define CAN_BS2_6TQ                 ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_0))  /*!< 6 time quantum */
#define CAN_BS2_7TQ                 ((uint32_t)(CAN_BTR_TS2_2 | CAN_BTR_TS2_1))  /*!< 7 time quantum */
#define CAN_BS2_8TQ                 ((uint32_t)CAN_BTR_TS2)                      /*!< 8 time quantum */


static void MX_CAN_Init(void)
{
    uint32_t timeout=0;
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    GPIOB->CRH &= ~GPIO_CRH_CNF9;
    GPIOB->CRH &= ~GPIO_CRH_CNF8;
    GPIOB->CRH &= ~GPIO_CRH_MODE8;
    GPIOB->CRH |= GPIO_CRH_CNF9_1|GPIO_CRH_MODE9|GPIO_CRH_CNF8_0;

    //    __HAL_AFIO_REMAP_CAN1_2();
    AFIO->MAPR |= AFIO_MAPR_CAN_REMAP_REMAP2;


    /* Exit from sleep mode */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_SLEEP);

    /* Request initialisation */
    SET_BIT(CAN1->MCR, CAN_MCR_INRQ);

    /* Wait initialisation acknowledge */
    timeout=0;
    while (((CAN1->MSR & CAN_MSR_INAK) == 0U)&&(timeout<10000))
    {
        timeout++;
    }

    /* Set the time triggered communication mode */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_TTCM); // TimeTriggeredMode = DISABLE;

    /* Set the automatic bus-off management */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_ABOM); // AutoBusOff = DISABLE;

    /* Set the automatic wake-up mode */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_AWUM); // AutoWakeUp = DISABLE;

    /* Set the automatic retransmission */
    SET_BIT(CAN1->MCR, CAN_MCR_NART);   // AutoRetransmission = DISABLE;

    /* Set the receive FIFO locked mode */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_RFLM); // ReceiveFifoLocked = DISABLE;

    /* Set the transmit FIFO priority */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_TXFP); // TransmitFifoPriority = DISABLE;

//    /* Set the bit timing register */
    CAN1->BTR = CAN_MODE_LOOPBACK|CAN_BS1_7TQ|CAN_BS2_1TQ|CAN_Prescaler;

//    WRITE_REG(CAN1->BTR, (uint32_t)(hcan->Init.Mode           |
//                                              hcan->Init.SyncJumpWidth  |
//                                              hcan->Init.TimeSeg1       |
//                                              hcan->Init.TimeSeg2       |
//                                              (hcan->Init.Prescaler - 1U)));

//    CAN_BTR_BRP
//    hcan.Instance = CAN1;
//    hcan.Init.Prescaler = 81;
//    hcan.Init.Mode = CAN_MODE_LOOPBACK;
//    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
//    hcan.Init.TimeSeg1 = CAN_BS1_7TQ;
//    hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
//    hcan.Init.TimeTriggeredMode = DISABLE;
//    hcan.Init.AutoBusOff = DISABLE;
//    hcan.Init.AutoWakeUp = DISABLE;
//    hcan.Init.AutoRetransmission = DISABLE;
//    hcan.Init.ReceiveFifoLocked = DISABLE;
//    hcan.Init.TransmitFifoPriority = DISABLE;

//    if (HAL_CAN_Init(&hcan) != HAL_OK)
//    {
//      Error_Handler();
//    }
}




void CAN_Config (void)
{
  CAN_FilterTypeDef sFilterConfig;

  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0;
  sFilterConfig.FilterIdLow = 0;
  sFilterConfig.FilterMaskIdHigh = 0;
  sFilterConfig.FilterMaskIdLow = 0;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

  HAL_CAN_Start (& hcan);

//  HAL_CAN_ActivateNotification (& hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

  TxHeader.StdId = 0x320;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 8;
//  TxHeader.TransmitGlobalTime = DISABLE;
}



void SystemClock_Config(void)
{
      RCC_OscInitTypeDef RCC_OscInitStruct = {0};
      RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
      RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

      /** Initializes the CPU, AHB and APB busses clocks
      */
      RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
      RCC_OscInitStruct.HSIState = RCC_HSI_ON;
      RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
      RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
      RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
      if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
      {
        Error_Handler();
      }
      /** Initializes the CPU, AHB and APB busses clocks
      */
      RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
      RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
      RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
      RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
      RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

      if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
      {
        Error_Handler();
      }
      PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
      PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
      if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
      {
        Error_Handler();
      }
}

static void MX_GPIO_Init(void)
{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};

//     /* GPIO Ports Clock Enable */
//     __HAL_RCC_GPIOC_CLK_ENABLE();
//     __HAL_RCC_GPIOA_CLK_ENABLE();
//     __HAL_RCC_GPIOB_CLK_ENABLE();
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH |= GPIO_CRH_MODE13;
//     /*Configure GPIO pin Output Level */
//     HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

//     /*Configure GPIO pin : PC13 */
//     GPIO_InitStruct.Pin = GPIO_PIN_13;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}


int main()
{
    uint8_t datatest[20]= "hello hello hello";
    uint32_t counttest=0,count=0;
    HAL_Init();
    HAL_MspInit();
    SystemClock_Config();
    MX_GPIO_Init();
    uart::CONTROLLER.init();
    can::CONTROLLER.init();
//    MX_CAN_Init();
    checkvalue = CAN1->BTR;
    CAN_Config();
    /* Initialize the UART state */
//    MX_USART1_UART_Init(48000000,115200);
    while (true)
    {
        counttest++;
        if(counttest<100000)
        {
            counttest++;
        }
        else
        {
            CAN_SendTxMessage(8,0x01,datatest);
            uart::CONTROLLER.printfMessage("hello123..%lx\r\n",checkvalue);
//            HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
            counttest=0;
        }
        GPIO::CONTROLLER.loop();
        uart::CONTROLLER.loop();
        HAL_CAN_GetRxMessage (&hcan, CAN_RX_FIFO0, & RxHeader, RxData);
        if (RxHeader.StdId == 0x01 && RxHeader.DLC == 8 && RxData [4] == 4)
        {
            uart::CONTROLLER.printfMessage("good msg: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",RxData[0], \
                                                                                               RxData[1], \
                                                                                               RxData[2], \
                                                                                               RxData[3], \
                                                                                               RxData[4], \
                                                                                               RxData[5], \
                                                                                               RxData[6], \
                                                                                               RxData[7]   );
              // Data processing
        }
    }
//    return 0;
}
