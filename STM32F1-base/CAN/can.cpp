#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "../uart/controller.h"
#include "can.h"


can::Controller can::CONTROLLER;

#define CAN_Prescaler   80

#define Can_FilterBank          0
#define Can_FilterIdHigh        0
#define Can_FilterIdLow         0
#define Can_FilterMaskIdHigh    0
#define Can_FilterMaskIdLow     0


void can::Controller::init()
{
    uint32_t timeout=0;
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

    /* Check Sleep mode leave acknowledge */
    timeout=0;
    while (((CAN1->MSR & CAN_MSR_SLAK) != 0U)&&(timeout<flagcheck_timeout))
    {
         timeout++;
    }

    /* Request initialisation */
    SET_BIT(CAN1->MCR, CAN_MCR_INRQ);

    /* Wait initialisation acknowledge */
    timeout=0;
    while (((CAN1->MSR & CAN_MSR_INAK) == 0U)&&(timeout<flagcheck_timeout))
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

//    can::CONTROLLER.CAN_ConfigFilter();
}


void can::Controller::loop()
{

}

void can::Controller::CAN_SendTxMessage(uint32_t DTLC, uint32_t addr, uint8_t *aData)
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

bool can::Controller::CAN_GetRxMessage(uint32_t DeviceID, uint8_t* aData)
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


void can::Controller::CAN_ConfigFilter()
{
    uint32_t filternbrbitpos;
    /* Initialisation mode for the filter */
    SET_BIT(CAN1->FMR, CAN_FMR_FINIT);
    /* Convert filter number into bit position */
    filternbrbitpos = (uint32_t)1 << (Can_FilterBank & 0x1FU);

    /* Filter Deactivation */
    CLEAR_BIT(CAN1->FA1R, filternbrbitpos);

    /* 32-bit scale for the filter */
    SET_BIT(CAN1->FS1R, filternbrbitpos);

    /* 32-bit identifier or First 32-bit identifier */
    CAN1->sFilterRegister[Can_FilterBank].FR1 =
    ((0x0000FFFFU & (uint32_t)Can_FilterIdHigh) << 16U) |
    (0x0000FFFFU & (uint32_t)Can_FilterIdLow);

    /* 32-bit mask or Second 32-bit identifier */
    CAN1->sFilterRegister[Can_FilterBank].FR2 =
    ((0x0000FFFFU & (uint32_t)Can_FilterMaskIdHigh) << 16U) |
    (0x0000FFFFU & (uint32_t)Can_FilterMaskIdLow);

    /* Filter Mode */
    /* Id/Mask mode for the filter*/
    CLEAR_BIT(CAN1->FM1R, filternbrbitpos);

    /* Filter FIFO assignment */
    CLEAR_BIT(CAN1->FFA1R, filternbrbitpos);

    /* Filter activation */
    SET_BIT(CAN1->FA1R, filternbrbitpos);

    /* Leave the initialisation mode for the filter */
    CLEAR_BIT(CAN1->FMR, CAN_FMR_FINIT);

    /* Request leave initialisation */
    CLEAR_BIT(CAN1->MCR, CAN_MCR_INRQ);

}
