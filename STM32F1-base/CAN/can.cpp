#include "stm32f103xb.h"
#include "stm32f1xx_hal_can.h"
#include "../uart/controller.h"
#include "can.h"


can::Controller can::CONTROLLER;

CAN_TxHeaderTypeDef hcan;
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

//  HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);

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

}


void can::Controller::loop()
{

}

uint8_t can::Controller::Transmit(uint32_t addr, uint32_t data_size, uint8_t *tab_data)
{

}


void can::Controller::SystemClock_Config()
{

}
