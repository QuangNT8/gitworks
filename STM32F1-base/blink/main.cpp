
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

    TxHeader.IDE = 0;
    TxHeader.StdId = addr;
    TxHeader.DLC = data_size;
    TxHeader.RTR =0;
    TxHeader.TransmitGlobalTime = DISABLE;

    HAL_CAN_AddTxMessage(&hcan, &TxHeader, tab_data, &TxMailbox);

    return 1;
}


static void MX_CAN_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_CAN1_CLK_ENABLE();

       __HAL_RCC_GPIOB_CLK_ENABLE();
       /**CAN GPIO Configuration
       PB8     ------> CAN_RX
       PB9     ------> CAN_TX
       */
       GPIO_InitStruct.Pin = GPIO_PIN_8;
       GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
       GPIO_InitStruct.Pull = GPIO_NOPULL;
       HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

       GPIO_InitStruct.Pin = GPIO_PIN_9;
       GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
       GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
       HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

       __HAL_AFIO_REMAP_CAN1_2();

    hcan.Instance = CAN1;
    hcan.Init.Prescaler = 80;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_7TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    HAL_CAN_Init(&hcan);
}




void CAN_Config (void)
{
    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x02 <<5;
    sFilterConfig.FilterIdLow = 0;
    sFilterConfig.FilterMaskIdHigh = 0xFFF<<5;
    sFilterConfig.FilterMaskIdLow = 0;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

    HAL_CAN_Start (& hcan);

    //  HAL_CAN_ActivateNotification (& hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

    TxHeader.StdId = 0x100;
    TxHeader.ExtId = 0x10244060;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_EXT;
    TxHeader.DLC = 8;
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

    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH |= GPIO_CRH_MODE13;

}

bool CAN_GetRxMessage(uint32_t DeviceID, uint8_t aData[])
{
    bool result = true;
    uint32_t IDE, StdId, RTR, DLC, FilterMatchIndex, Timestamp;

    /* Check that the Rx FIFO 0 is not empty */
    if ((CAN1->RF0R & CAN_RF0R_FMP0) == 0U)
    {
        /* Update error code */
    }

    /* Get the header */
    IDE = CAN_RI0R_IDE & CAN1->sFIFOMailBox[0].RIR;
    if (IDE == (uint32_t)0)
    {
        StdId = (CAN_RI0R_STID & CAN1->sFIFOMailBox[0].RIR) >> CAN_TI0R_STID_Pos;
    }
    else
    {
        StdId = ((CAN_RI0R_EXID | CAN_RI0R_STID) & CAN1->sFIFOMailBox[0].RIR) >> CAN_RI0R_EXID_Pos;
    }
    if( DeviceID == StdId )
    {
        RTR = (CAN_RI0R_RTR & CAN1->sFIFOMailBox[0].RIR);
        DLC = (CAN_RDT0R_DLC & CAN1->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_DLC_Pos;
        FilterMatchIndex = (CAN_RDT0R_FMI & CAN1->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_FMI_Pos;
        Timestamp = (CAN_RDT0R_TIME & CAN1->sFIFOMailBox[0].RDTR) >> CAN_RDT0R_TIME_Pos;

        /* Get the data */
        aData[0] = (uint8_t)((CAN_RDL0R_DATA0 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA0_Pos);
        aData[1] = (uint8_t)((CAN_RDL0R_DATA1 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA1_Pos);
        aData[2] = (uint8_t)((CAN_RDL0R_DATA2 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA2_Pos);
        aData[3] = (uint8_t)((CAN_RDL0R_DATA3 & CAN1->sFIFOMailBox[0].RDLR) >> CAN_RDL0R_DATA3_Pos);
        aData[4] = (uint8_t)((CAN_RDH0R_DATA4 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA4_Pos);
        aData[5] = (uint8_t)((CAN_RDH0R_DATA5 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA5_Pos);
        aData[6] = (uint8_t)((CAN_RDH0R_DATA6 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA6_Pos);
        aData[7] = (uint8_t)((CAN_RDH0R_DATA7 & CAN1->sFIFOMailBox[0].RDHR) >> CAN_RDH0R_DATA7_Pos);
        result = true;
    }
    else
    {
        result = false;
    }
    /* Release RX FIFO 0 */
    SET_BIT(CAN1->RF0R, CAN_RF0R_RFOM0);

    return result;
}


int main()
{
    uint8_t datatest[8]= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
    uint32_t counttest=0,count=0;
    HAL_Init();
    HAL_MspInit();
    SystemClock_Config();
    MX_GPIO_Init();

    uart::CONTROLLER.init();

//    can::CONTROLLER.init();
    MX_CAN_Init();
    CAN_Config();
//    can::CONTROLLER.ConfigFilter();
    /* Initialize the UART state */
//    MX_USART1_UART_Init(48000000,115200);
    checkvalue = CAN1->FM1R;

    while (true)
    {
        counttest++;
        if(counttest<150000)
        {
            counttest++;
        }
        else
        {
            CAN_SendTxMessage(8,0x01,datatest);
//            CAN_Transmit(0x01,8,datatest);
//            uart::CONTROLLER.printfMessage("hello123..%lx\r\n",RxHeader.StdId);
            HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
            counttest=0;
        }
//        GPIO::CONTROLLER.loop();
        uart::CONTROLLER.loop();
//        uart::CONTROLLER.printfMessage("hello123..%lx\r\n",RxHeader.StdId);
#if 1
//        if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)> 0)

    //        uart::CONTROLLER.printfMessage("RxHeader.IDE 0x%x",RxHeader.IDE);
    //        uart::CONTROLLER.printfMessage("RxHeader.StdId 0x%x",RxHeader.StdId);
        if(can::CONTROLLER.GetRxMessage(0x01,RxData)==true)
        {
            uart::CONTROLLER.printfMessage("good msg: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",RxData[0], \
                                                                                               RxData[1], \
                                                                                               RxData[2], \
                                                                                               RxData[3], \
                                                                                               RxData[4], \
                                                                                               RxData[5], \
                                                                                               RxData[6], \
                                                                                               RxData[7]   );
        }
#endif
    }
//    return 0;
}
