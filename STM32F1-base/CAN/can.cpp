
#include "../uart/controller.h"
#include "can.h"
#include "stm32f0xx_hal.h"
#include "STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_can.h"

can::Controller can::CONTROLLER;

CAN_HandleTypeDef hcan;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

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

  TxHeader.StdId = 0x01;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 8;
//  TxHeader.TransmitGlobalTime = DISABLE;
}


void can::Controller::init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
     /* System interrupt init*/
    __HAL_REMAP_PIN_ENABLE(HAL_REMAP_PA11_PA12);

    HAL_Init();

    __HAL_RCC_CAN1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_CAN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE END CAN_Init 1 */
    hcan.Instance = CAN;
    hcan.Init.Prescaler = 90;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_11TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    HAL_CAN_Init(&hcan);

    CAN_Config();
}

extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

void can::Controller::loop()
{
    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)> 0)
    {
//        uart::CONTROLLER.printfMessage("RxHeader.IDE 0x%x",RxHeader.IDE);
//        uart::CONTROLLER.printfMessage("RxHeader.StdId 0x%x",RxHeader.StdId);
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
}

uint8_t can::Controller::Transmit(uint32_t addr, uint32_t data_size, uint8_t *tab_data)
{
    TxHeader.StdId = addr;
    TxHeader.DLC = data_size;

    for(int i = 0; i < data_size; i++)
    {
       TxData[i] = tab_data[i];
    }
    HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
    return 1;
}

void can::Controller::delay(uint16_t dl)
{
    HAL_Delay(dl);
}

void can::Controller::SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
    //       Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
    //       Error_Handler();
    }
}
