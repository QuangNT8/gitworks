/**
  ******************************************************************************
  * @file    stm32f051x8.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M0 Device Peripheral Access Layer Header File. 
  *          This file contains all the peripheral register's definitions, bits 
  *          definitions and memory mapping for STM32F0xx devices.            
  *            
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral's registers declarations and bits definition
  *           - Macros to access peripheral’s registers hardware
  *  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
                                                           
/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f051x8
  * @{
  */
    
#ifndef __STM32F051x8_H
#define __STM32F051x8_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

  /** @addtogroup Configuration_section_for_CMSIS
  * @{
  */
/**
 * @brief Configuration of the Cortex-M0 Processor and Core Peripherals
 */
//#define __CM0_REV                 0 /*!< Core Revision r0p0                            */
//#define __MPU_PRESENT             0 /*!< STM32F0xx do not provide MPU                  */
//#define __NVIC_PRIO_BITS          2 /*!< STM32F0xx uses 2 Bits for the Priority Levels */
//#define __Vendor_SysTickConfig    0     /*!< Set to 1 if different SysTick Config is used */
 
/*******************  Bit definition for RCC_CFGR3 register  *****************/
/*!< USART1 Clock source selection */
#define RCC_CFGR3_USART1SW_Pos                   (0U)                          
#define RCC_CFGR3_USART1SW_Msk                   (0x3U << RCC_CFGR3_USART1SW_Pos) /*!< 0x00000003 */

#define RCC_CFGR3_USART1SW_PCLK                  (0x00000000U)                 /*!< PCLK clock used as USART1 clock source */
#define RCC_CFGR3_USART1SW_SYSCLK                (0x00000001U)                 /*!< System clock selected as USART1 clock source */
#define RCC_CFGR3_USART1SW_LSE                   (0x00000002U)                 /*!< LSE oscillator clock used as USART1 clock source */
#define RCC_CFGR3_USART1SW_HSI                   (0x00000003U)                 /*!< HSI oscillator clock used as USART1 clock source */

/*******************  Bit definition for I2C_CR1 register  *******************/
#define I2C_CR1_PE_Pos               (0U)                                      
#define I2C_CR1_PE_Msk               (0x1U << I2C_CR1_PE_Pos)                  /*!< 0x00000001 */
#define I2C_CR1_PE                   I2C_CR1_PE_Msk                            /*!< Peripheral enable */
#define I2C_CR1_TXIE_Pos             (1U)                                      
#define I2C_CR1_TXIE_Msk             (0x1U << I2C_CR1_TXIE_Pos)                /*!< 0x00000002 */
#define I2C_CR1_TXIE                 I2C_CR1_TXIE_Msk                          /*!< TX interrupt enable */
#define I2C_CR1_RXIE_Pos             (2U)                                      
#define I2C_CR1_RXIE_Msk             (0x1U << I2C_CR1_RXIE_Pos)                /*!< 0x00000004 */
#define I2C_CR1_RXIE                 I2C_CR1_RXIE_Msk                          /*!< RX interrupt enable */
#define I2C_CR1_ADDRIE_Pos           (3U)                                      
#define I2C_CR1_ADDRIE_Msk           (0x1U << I2C_CR1_ADDRIE_Pos)              /*!< 0x00000008 */
#define I2C_CR1_ADDRIE               I2C_CR1_ADDRIE_Msk                        /*!< Address match interrupt enable */
#define I2C_CR1_NACKIE_Pos           (4U)                                      
#define I2C_CR1_NACKIE_Msk           (0x1U << I2C_CR1_NACKIE_Pos)              /*!< 0x00000010 */
#define I2C_CR1_NACKIE               I2C_CR1_NACKIE_Msk                        /*!< NACK received interrupt enable */
#define I2C_CR1_STOPIE_Pos           (5U)                                      
#define I2C_CR1_STOPIE_Msk           (0x1U << I2C_CR1_STOPIE_Pos)              /*!< 0x00000020 */
#define I2C_CR1_STOPIE               I2C_CR1_STOPIE_Msk                        /*!< STOP detection interrupt enable */
#define I2C_CR1_TCIE_Pos             (6U)                                      
#define I2C_CR1_TCIE_Msk             (0x1U << I2C_CR1_TCIE_Pos)                /*!< 0x00000040 */
#define I2C_CR1_TCIE                 I2C_CR1_TCIE_Msk                          /*!< Transfer complete interrupt enable */
#define I2C_CR1_ERRIE_Pos            (7U)                                      
#define I2C_CR1_ERRIE_Msk            (0x1U << I2C_CR1_ERRIE_Pos)               /*!< 0x00000080 */
#define I2C_CR1_ERRIE                I2C_CR1_ERRIE_Msk                         /*!< Errors interrupt enable */
#define I2C_CR1_DNF_Pos              (8U)                                      
#define I2C_CR1_DNF_Msk              (0xFU << I2C_CR1_DNF_Pos)                 /*!< 0x00000F00 */
#define I2C_CR1_DNF                  I2C_CR1_DNF_Msk                           /*!< Digital noise filter */
#define I2C_CR1_ANFOFF_Pos           (12U)                                     
#define I2C_CR1_ANFOFF_Msk           (0x1U << I2C_CR1_ANFOFF_Pos)              /*!< 0x00001000 */
#define I2C_CR1_ANFOFF               I2C_CR1_ANFOFF_Msk                        /*!< Analog noise filter OFF */
#define I2C_CR1_SWRST_Pos            (13U)                                     
#define I2C_CR1_SWRST_Msk            (0x1U << I2C_CR1_SWRST_Pos)               /*!< 0x00002000 */
#define I2C_CR1_SWRST                I2C_CR1_SWRST_Msk                         /*!< Software reset */
#define I2C_CR1_TXDMAEN_Pos          (14U)                                     
#define I2C_CR1_TXDMAEN_Msk          (0x1U << I2C_CR1_TXDMAEN_Pos)             /*!< 0x00004000 */
#define I2C_CR1_TXDMAEN              I2C_CR1_TXDMAEN_Msk                       /*!< DMA transmission requests enable */
#define I2C_CR1_RXDMAEN_Pos          (15U)                                     
#define I2C_CR1_RXDMAEN_Msk          (0x1U << I2C_CR1_RXDMAEN_Pos)             /*!< 0x00008000 */
#define I2C_CR1_RXDMAEN              I2C_CR1_RXDMAEN_Msk                       /*!< DMA reception requests enable */
#define I2C_CR1_SBC_Pos              (16U)                                     
#define I2C_CR1_SBC_Msk              (0x1U << I2C_CR1_SBC_Pos)                 /*!< 0x00010000 */
#define I2C_CR1_SBC                  I2C_CR1_SBC_Msk                           /*!< Slave byte control */
#define I2C_CR1_NOSTRETCH_Pos        (17U)                                     
#define I2C_CR1_NOSTRETCH_Msk        (0x1U << I2C_CR1_NOSTRETCH_Pos)           /*!< 0x00020000 */
#define I2C_CR1_NOSTRETCH            I2C_CR1_NOSTRETCH_Msk                     /*!< Clock stretching disable */
#define I2C_CR1_WUPEN_Pos            (18U)                                     
#define I2C_CR1_WUPEN_Msk            (0x1U << I2C_CR1_WUPEN_Pos)               /*!< 0x00040000 */
#define I2C_CR1_WUPEN                I2C_CR1_WUPEN_Msk                         /*!< Wakeup from STOP enable */
#define I2C_CR1_GCEN_Pos             (19U)                                     
#define I2C_CR1_GCEN_Msk             (0x1U << I2C_CR1_GCEN_Pos)                /*!< 0x00080000 */
#define I2C_CR1_GCEN                 I2C_CR1_GCEN_Msk                          /*!< General call enable */
#define I2C_CR1_SMBHEN_Pos           (20U)                                     
#define I2C_CR1_SMBHEN_Msk           (0x1U << I2C_CR1_SMBHEN_Pos)              /*!< 0x00100000 */
#define I2C_CR1_SMBHEN               I2C_CR1_SMBHEN_Msk                        /*!< SMBus host address enable */
#define I2C_CR1_SMBDEN_Pos           (21U)                                     
#define I2C_CR1_SMBDEN_Msk           (0x1U << I2C_CR1_SMBDEN_Pos)              /*!< 0x00200000 */
#define I2C_CR1_SMBDEN               I2C_CR1_SMBDEN_Msk                        /*!< SMBus device default address enable */
#define I2C_CR1_ALERTEN_Pos          (22U)                                     
#define I2C_CR1_ALERTEN_Msk          (0x1U << I2C_CR1_ALERTEN_Pos)             /*!< 0x00400000 */
#define I2C_CR1_ALERTEN              I2C_CR1_ALERTEN_Msk                       /*!< SMBus alert enable */
#define I2C_CR1_PECEN_Pos            (23U)                                     
#define I2C_CR1_PECEN_Msk            (0x1U << I2C_CR1_PECEN_Pos)               /*!< 0x00800000 */
#define I2C_CR1_PECEN                I2C_CR1_PECEN_Msk                         /*!< PEC enable */

/********************  Bit definition for PWR_CR register  *******************/
#define PWR_CR_LPDS_Pos            (0U)                                        
#define PWR_CR_LPDS_Msk            (0x1U << PWR_CR_LPDS_Pos)                   /*!< 0x00000001 */
#define PWR_CR_LPDS                PWR_CR_LPDS_Msk                             /*!< Low-power Deepsleep */
#define PWR_CR_PDDS_Pos            (1U)                                        
#define PWR_CR_PDDS_Msk            (0x1U << PWR_CR_PDDS_Pos)                   /*!< 0x00000002 */
#define PWR_CR_PDDS                PWR_CR_PDDS_Msk                             /*!< Power Down Deepsleep */
#define PWR_CR_CWUF_Pos            (2U)                                        
#define PWR_CR_CWUF_Msk            (0x1U << PWR_CR_CWUF_Pos)                   /*!< 0x00000004 */
#define PWR_CR_CWUF                PWR_CR_CWUF_Msk                             /*!< Clear Wakeup Flag */
#define PWR_CR_CSBF_Pos            (3U)                                        
#define PWR_CR_CSBF_Msk            (0x1U << PWR_CR_CSBF_Pos)                   /*!< 0x00000008 */
#define PWR_CR_CSBF                PWR_CR_CSBF_Msk                             /*!< Clear Standby Flag */
#define PWR_CR_PVDE_Pos            (4U)                                        
#define PWR_CR_PVDE_Msk            (0x1U << PWR_CR_PVDE_Pos)                   /*!< 0x00000010 */
#define PWR_CR_PVDE                PWR_CR_PVDE_Msk                             /*!< Power Voltage Detector Enable */

#define PWR_CR_PLS_Pos             (5U)                                        
#define PWR_CR_PLS_Msk             (0x7U << PWR_CR_PLS_Pos)                    /*!< 0x000000E0 */
#define PWR_CR_PLS                 PWR_CR_PLS_Msk                              /*!< PLS[2:0] bits (PVD Level Selection) */
#define PWR_CR_PLS_0               (0x1U << PWR_CR_PLS_Pos)                    /*!< 0x00000020 */
#define PWR_CR_PLS_1               (0x2U << PWR_CR_PLS_Pos)                    /*!< 0x00000040 */
#define PWR_CR_PLS_2               (0x4U << PWR_CR_PLS_Pos)                    /*!< 0x00000080 */

/*!< AHB peripherals */
#define DMA1_BASE             (AHBPERIPH_BASE + 0x00000000)
#define DMA1_Channel1_BASE    (DMA1_BASE + 0x00000008)
#define DMA1_Channel2_BASE    (DMA1_BASE + 0x0000001C)
#define DMA1_Channel3_BASE    (DMA1_BASE + 0x00000030)
#define DMA1_Channel4_BASE    (DMA1_BASE + 0x00000044)
#define DMA1_Channel5_BASE    (DMA1_BASE + 0x00000058)


#define RCC_BASE              (AHBPERIPH_BASE + 0x00001000)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000) /*!< FLASH registers base address */
#define OB_BASE               ((uint32_t)0x1FFFF800U)       /*!< FLASH Option Bytes base address */
#define FLASHSIZE_BASE        ((uint32_t)0x1FFFF7CCU)       /*!< FLASH Size register base address */
#define UID_BASE              ((uint32_t)0x1FFFF7ACU)       /*!< Unique device ID register base address */
#define CRC_BASE              (AHBPERIPH_BASE + 0x00003000)
#define TSC_BASE              (AHBPERIPH_BASE + 0x00004000)


/******************  Bit definition for FLASH_OBR register  *******************/
#define FLASH_OBR_OPTERR_Pos              (0U)                                 
#define FLASH_OBR_OPTERR_Msk              (0x1U << FLASH_OBR_OPTERR_Pos)       /*!< 0x00000001 */
#define FLASH_OBR_OPTERR                  FLASH_OBR_OPTERR_Msk                 /*!< Option Byte Error */
#define FLASH_OBR_RDPRT1_Pos              (1U)                                 
#define FLASH_OBR_RDPRT1_Msk              (0x1U << FLASH_OBR_RDPRT1_Pos)       /*!< 0x00000002 */
#define FLASH_OBR_RDPRT1                  FLASH_OBR_RDPRT1_Msk                 /*!< Read protection Level 1 */
#define FLASH_OBR_RDPRT2_Pos              (2U)                                 
#define FLASH_OBR_RDPRT2_Msk              (0x1U << FLASH_OBR_RDPRT2_Pos)       /*!< 0x00000004 */
#define FLASH_OBR_RDPRT2                  FLASH_OBR_RDPRT2_Msk                 /*!< Read protection Level 2 */

#define FLASH_OBR_USER_Pos                (8U)                                 
#define FLASH_OBR_USER_Msk                (0x77U << FLASH_OBR_USER_Pos)        /*!< 0x00007700 */
#define FLASH_OBR_USER                    FLASH_OBR_USER_Msk                   /*!< User Option Bytes */
#define FLASH_OBR_IWDG_SW_Pos             (8U)                                 
#define FLASH_OBR_IWDG_SW_Msk             (0x1U << FLASH_OBR_IWDG_SW_Pos)      /*!< 0x00000100 */
#define FLASH_OBR_IWDG_SW                 FLASH_OBR_IWDG_SW_Msk                /*!< IWDG SW */
#define FLASH_OBR_nRST_STOP_Pos           (9U)                                 
#define FLASH_OBR_nRST_STOP_Msk           (0x1U << FLASH_OBR_nRST_STOP_Pos)    /*!< 0x00000200 */
#define FLASH_OBR_nRST_STOP               FLASH_OBR_nRST_STOP_Msk              /*!< nRST_STOP */
#define FLASH_OBR_nRST_STDBY_Pos          (10U)                                
#define FLASH_OBR_nRST_STDBY_Msk          (0x1U << FLASH_OBR_nRST_STDBY_Pos)   /*!< 0x00000400 */
#define FLASH_OBR_nRST_STDBY              FLASH_OBR_nRST_STDBY_Msk             /*!< nRST_STDBY */
#define FLASH_OBR_nBOOT1_Pos              (12U)                                
#define FLASH_OBR_nBOOT1_Msk              (0x1U << FLASH_OBR_nBOOT1_Pos)       /*!< 0x00001000 */
#define FLASH_OBR_nBOOT1                  FLASH_OBR_nBOOT1_Msk                 /*!< nBOOT1 */
#define FLASH_OBR_VDDA_MONITOR_Pos        (13U)                                
#define FLASH_OBR_VDDA_MONITOR_Msk        (0x1U << FLASH_OBR_VDDA_MONITOR_Pos) /*!< 0x00002000 */
#define FLASH_OBR_VDDA_MONITOR            FLASH_OBR_VDDA_MONITOR_Msk           /*!< VDDA power supply supervisor */
#define FLASH_OBR_RAM_PARITY_CHECK_Pos    (14U)                                
#define FLASH_OBR_RAM_PARITY_CHECK_Msk    (0x1U << FLASH_OBR_RAM_PARITY_CHECK_Pos) /*!< 0x00004000 */
#define FLASH_OBR_RAM_PARITY_CHECK        FLASH_OBR_RAM_PARITY_CHECK_Msk       /*!< RAM parity check */
#define FLASH_OBR_DATA0_Pos               (16U)                                
#define FLASH_OBR_DATA0_Msk               (0xFFU << FLASH_OBR_DATA0_Pos)       /*!< 0x00FF0000 */
#define FLASH_OBR_DATA0                   FLASH_OBR_DATA0_Msk                  /*!< Data0 */
#define FLASH_OBR_DATA1_Pos               (24U)                                
#define FLASH_OBR_DATA1_Msk               (0xFFU << FLASH_OBR_DATA1_Pos)       /*!< 0xFF000000 */
#define FLASH_OBR_DATA1                   FLASH_OBR_DATA1_Msk                  /*!< Data1 */


/******************  FLASH Keys  **********************************************/
#define FLASH_KEY1_Pos                    (0U)                                 
#define FLASH_KEY1_Msk                    (0x45670123U << FLASH_KEY1_Pos)      /*!< 0x45670123 */
#define FLASH_KEY1                        FLASH_KEY1_Msk                       /*!< Flash program erase key1 */
#define FLASH_KEY2_Pos                    (0U)                                 
#define FLASH_KEY2_Msk                    (0xCDEF89ABU << FLASH_KEY2_Pos)      /*!< 0xCDEF89AB */
#define FLASH_KEY2                        FLASH_KEY2_Msk                       /*!< Flash program erase key2: used with FLASH_PEKEY1

/******************  Bit definition for ADC_SMPR register  ********************/
#define ADC_SMPR_SMP_Pos          (0U)                                         
#define ADC_SMPR_SMP_Msk          (0x7U << ADC_SMPR_SMP_Pos)                   /*!< 0x00000007 */
#define ADC_SMPR_SMP              ADC_SMPR_SMP_Msk                             /*!< ADC group of channels sampling time 2 */
#define ADC_SMPR_SMP_0            (0x1U << ADC_SMPR_SMP_Pos)                   /*!< 0x00000001 */
#define ADC_SMPR_SMP_1            (0x2U << ADC_SMPR_SMP_Pos)                   /*!< 0x00000002 */
#define ADC_SMPR_SMP_2            (0x4U << ADC_SMPR_SMP_Pos)                   /*!< 0x00000004 */

/* Legacy defines */
#define  ADC_SMPR1_SMPR         (ADC_SMPR_SMP)         /*!< SMP[2:0] bits (Sampling time selection) */
#define  ADC_SMPR1_SMPR_0       (ADC_SMPR_SMP_0)       /*!< bit 0 */
#define  ADC_SMPR1_SMPR_1       (ADC_SMPR_SMP_1)       /*!< bit 1 */
#define  ADC_SMPR1_SMPR_2       (ADC_SMPR_SMP_2)       /*!< bit 2 */                                                                                to unlock the write access to the FPEC. */


#define COMP1               ((COMP_TypeDef *) COMP_BASE)
#define COMP2               ((COMP_TypeDef *) (COMP_BASE + 0x00000002))

/*******************  Bits definition for ADC_CFGR2 register  *****************/
#define ADC_CFGR2_CKMODE_Pos      (30U)                                        
#define ADC_CFGR2_CKMODE_Msk      (0x3U << ADC_CFGR2_CKMODE_Pos)               /*!< 0xC0000000 */
#define ADC_CFGR2_CKMODE          ADC_CFGR2_CKMODE_Msk                         /*!< ADC clock source and prescaler (prescaler only for clock source synchronous) */
#define ADC_CFGR2_CKMODE_1        (0x2U << ADC_CFGR2_CKMODE_Pos)               /*!< 0x80000000 */
#define ADC_CFGR2_CKMODE_0        (0x1U << ADC_CFGR2_CKMODE_Pos)               /*!< 0x40000000 */

/*******************  Bit definition for ADC_TR register  ********************/
#define ADC_TR1_LT1_Pos           (0U)                                         
#define ADC_TR1_LT1_Msk           (0xFFFU << ADC_TR1_LT1_Pos)                  /*!< 0x00000FFF */
#define ADC_TR1_LT1               ADC_TR1_LT1_Msk                              /*!< ADC analog watchdog 1 threshold low */
#define ADC_TR1_LT1_0             (0x001U << ADC_TR1_LT1_Pos)                  /*!< 0x00000001 */
#define ADC_TR1_LT1_1             (0x002U << ADC_TR1_LT1_Pos)                  /*!< 0x00000002 */
#define ADC_TR1_LT1_2             (0x004U << ADC_TR1_LT1_Pos)                  /*!< 0x00000004 */
#define ADC_TR1_LT1_3             (0x008U << ADC_TR1_LT1_Pos)                  /*!< 0x00000008 */
#define ADC_TR1_LT1_4             (0x010U << ADC_TR1_LT1_Pos)                  /*!< 0x00000010 */
#define ADC_TR1_LT1_5             (0x020U << ADC_TR1_LT1_Pos)                  /*!< 0x00000020 */
#define ADC_TR1_LT1_6             (0x040U << ADC_TR1_LT1_Pos)                  /*!< 0x00000040 */
#define ADC_TR1_LT1_7             (0x080U << ADC_TR1_LT1_Pos)                  /*!< 0x00000080 */
#define ADC_TR1_LT1_8             (0x100U << ADC_TR1_LT1_Pos)                  /*!< 0x00000100 */
#define ADC_TR1_LT1_9             (0x200U << ADC_TR1_LT1_Pos)                  /*!< 0x00000200 */
#define ADC_TR1_LT1_10            (0x400U << ADC_TR1_LT1_Pos)                  /*!< 0x00000400 */
#define ADC_TR1_LT1_11            (0x800U << ADC_TR1_LT1_Pos)                  /*!< 0x00000800 */

#define ADC_TR1_HT1_Pos           (16U)                                        
#define ADC_TR1_HT1_Msk           (0xFFFU << ADC_TR1_HT1_Pos)                  /*!< 0x0FFF0000 */
#define ADC_TR1_HT1               ADC_TR1_HT1_Msk                              /*!< ADC Analog watchdog 1 threshold high */
#define ADC_TR1_HT1_0             (0x001U << ADC_TR1_HT1_Pos)                  /*!< 0x00010000 */
#define ADC_TR1_HT1_1             (0x002U << ADC_TR1_HT1_Pos)                  /*!< 0x00020000 */
#define ADC_TR1_HT1_2             (0x004U << ADC_TR1_HT1_Pos)                  /*!< 0x00040000 */
#define ADC_TR1_HT1_3             (0x008U << ADC_TR1_HT1_Pos)                  /*!< 0x00080000 */
#define ADC_TR1_HT1_4             (0x010U << ADC_TR1_HT1_Pos)                  /*!< 0x00100000 */
#define ADC_TR1_HT1_5             (0x020U << ADC_TR1_HT1_Pos)                  /*!< 0x00200000 */
#define ADC_TR1_HT1_6             (0x040U << ADC_TR1_HT1_Pos)                  /*!< 0x00400000 */
#define ADC_TR1_HT1_7             (0x080U << ADC_TR1_HT1_Pos)                  /*!< 0x00800000 */
#define ADC_TR1_HT1_8             (0x100U << ADC_TR1_HT1_Pos)                  /*!< 0x01000000 */
#define ADC_TR1_HT1_9             (0x200U << ADC_TR1_HT1_Pos)                  /*!< 0x02000000 */
#define ADC_TR1_HT1_10            (0x400U << ADC_TR1_HT1_Pos)                  /*!< 0x04000000 */
#define ADC_TR1_HT1_11            (0x800U << ADC_TR1_HT1_Pos)                  /*!< 0x08000000 */

/* Legacy defines */
#define  ADC_TR_HT              (ADC_TR1_HT1)
#define  ADC_TR_LT              (ADC_TR1_LT1)
#define  ADC_HTR_HT             (ADC_TR1_HT1)
#define  ADC_LTR_LT             (ADC_TR1_LT1)

/* COMPx bits definition */
#define COMP_CSR_COMPxEN_Pos          (0U)                                     
#define COMP_CSR_COMPxEN_Msk          (0x1U << COMP_CSR_COMPxEN_Pos)           /*!< 0x00000001 */
#define COMP_CSR_COMPxEN              COMP_CSR_COMPxEN_Msk                     /*!< COMPx enable */
#define COMP_CSR_COMPxMODE_Pos        (2U)                                     
#define COMP_CSR_COMPxMODE_Msk        (0x3U << COMP_CSR_COMPxMODE_Pos)         /*!< 0x0000000C */
#define COMP_CSR_COMPxMODE            COMP_CSR_COMPxMODE_Msk                   /*!< COMPx power mode */
#define COMP_CSR_COMPxMODE_0          (0x1U << COMP_CSR_COMPxMODE_Pos)         /*!< 0x00000004 */
#define COMP_CSR_COMPxMODE_1          (0x2U << COMP_CSR_COMPxMODE_Pos)         /*!< 0x00000008 */
#define COMP_CSR_COMPxINSEL_Pos       (4U)                                     
#define COMP_CSR_COMPxINSEL_Msk       (0x7U << COMP_CSR_COMPxINSEL_Pos)        /*!< 0x00000070 */
#define COMP_CSR_COMPxINSEL           COMP_CSR_COMPxINSEL_Msk                  /*!< COMPx inverting input select */
#define COMP_CSR_COMPxINSEL_0         (0x1U << COMP_CSR_COMPxINSEL_Pos)        /*!< 0x00000010 */
#define COMP_CSR_COMPxINSEL_1         (0x2U << COMP_CSR_COMPxINSEL_Pos)        /*!< 0x00000020 */
#define COMP_CSR_COMPxINSEL_2         (0x4U << COMP_CSR_COMPxINSEL_Pos)        /*!< 0x00000040 */
#define COMP_CSR_COMPxOUTSEL_Pos      (8U)                                     
#define COMP_CSR_COMPxOUTSEL_Msk      (0x7U << COMP_CSR_COMPxOUTSEL_Pos)       /*!< 0x00000700 */
#define COMP_CSR_COMPxOUTSEL          COMP_CSR_COMPxOUTSEL_Msk                 /*!< COMPx output select */
#define COMP_CSR_COMPxOUTSEL_0        (0x1U << COMP_CSR_COMPxOUTSEL_Pos)       /*!< 0x00000100 */
#define COMP_CSR_COMPxOUTSEL_1        (0x2U << COMP_CSR_COMPxOUTSEL_Pos)       /*!< 0x00000200 */
#define COMP_CSR_COMPxOUTSEL_2        (0x4U << COMP_CSR_COMPxOUTSEL_Pos)       /*!< 0x00000400 */
#define COMP_CSR_COMPxPOL_Pos         (11U)                                    
#define COMP_CSR_COMPxPOL_Msk         (0x1U << COMP_CSR_COMPxPOL_Pos)          /*!< 0x00000800 */
#define COMP_CSR_COMPxPOL             COMP_CSR_COMPxPOL_Msk                    /*!< COMPx output polarity */
#define COMP_CSR_COMPxHYST_Pos        (12U)                                    
#define COMP_CSR_COMPxHYST_Msk        (0x3U << COMP_CSR_COMPxHYST_Pos)         /*!< 0x00003000 */
#define COMP_CSR_COMPxHYST            COMP_CSR_COMPxHYST_Msk                   /*!< COMPx hysteresis */
#define COMP_CSR_COMPxHYST_0          (0x1U << COMP_CSR_COMPxHYST_Pos)         /*!< 0x00001000 */
#define COMP_CSR_COMPxHYST_1          (0x2U << COMP_CSR_COMPxHYST_Pos)         /*!< 0x00002000 */
#define COMP_CSR_COMPxOUT_Pos         (14U)                                    
#define COMP_CSR_COMPxOUT_Msk         (0x1U << COMP_CSR_COMPxOUT_Pos)          /*!< 0x00004000 */
#define COMP_CSR_COMPxOUT             COMP_CSR_COMPxOUT_Msk                    /*!< COMPx output level */
#define COMP_CSR_COMPxLOCK_Pos        (15U)                                    
#define COMP_CSR_COMPxLOCK_Msk        (0x1U << COMP_CSR_COMPxLOCK_Pos)         /*!< 0x00008000 */
#define COMP_CSR_COMPxLOCK            COMP_CSR_COMPxLOCK_Msk                   /*!< COMPx lock */

/*!< CEC Clock source selection */
#define RCC_CFGR3_CECSW_Pos                      (6U)                          
#define RCC_CFGR3_CECSW_Msk                      (0x1U << RCC_CFGR3_CECSW_Pos) /*!< 0x00000040 */
#define RCC_CFGR3_CECSW                          RCC_CFGR3_CECSW_Msk           /*!< CECSW bits */ 

#define RCC_CFGR3_CECSW_HSI_DIV244               (0x00000000U)                 /*!< HSI clock divided by 244 selected as HDMI CEC entry clock source */
#define RCC_CFGR3_CECSW_LSE_Pos                  (6U)                          
#define RCC_CFGR3_CECSW_LSE_Msk                  (0x1U << RCC_CFGR3_CECSW_LSE_Pos) /*!< 0x00000040 */
#define RCC_CFGR3_CECSW_LSE                      RCC_CFGR3_CECSW_LSE_Msk       /*!< LSE clock selected as HDMI CEC entry clock source */

/*!< I2C1 Clock source selection */
#define RCC_CFGR3_I2C1SW_Pos                     (4U)                          
#define RCC_CFGR3_I2C1SW_Msk                     (0x1U << RCC_CFGR3_I2C1SW_Pos) /*!< 0x00000010 */
#define RCC_CFGR3_I2C1SW                         RCC_CFGR3_I2C1SW_Msk          /*!< I2C1SW bits */ 

#define RCC_CFGR3_I2C1SW_HSI                     (0x00000000U)                 /*!< HSI oscillator clock used as I2C1 clock source */
#define RCC_CFGR3_I2C1SW_SYSCLK_Pos              (4U)                          
#define RCC_CFGR3_I2C1SW_SYSCLK_Msk              (0x1U << RCC_CFGR3_I2C1SW_SYSCLK_Pos) /*!< 0x00000010 */
#define RCC_CFGR3_I2C1SW_SYSCLK                  RCC_CFGR3_I2C1SW_SYSCLK_Msk   /*!< System clock selected as I2C1 clock source */

/********************  Bit definition for RCC_CR register  *******************/
#define RCC_CR_HSION_Pos                         (0U)                          
#define RCC_CR_HSION_Msk                         (0x1U << RCC_CR_HSION_Pos)    /*!< 0x00000001 */
#define RCC_CR_HSION                             RCC_CR_HSION_Msk              /*!< Internal High Speed clock enable */
#define RCC_CR_HSIRDY_Pos                        (1U)                          
#define RCC_CR_HSIRDY_Msk                        (0x1U << RCC_CR_HSIRDY_Pos)   /*!< 0x00000002 */
#define RCC_CR_HSIRDY                            RCC_CR_HSIRDY_Msk             /*!< Internal High Speed clock ready flag */

#define RCC_CR_HSITRIM_Pos                       (3U)                          
#define RCC_CR_HSITRIM_Msk                       (0x1FU << RCC_CR_HSITRIM_Pos) /*!< 0x000000F8 */
#define RCC_CR_HSITRIM                           RCC_CR_HSITRIM_Msk            /*!< Internal High Speed clock trimming */
#define RCC_CR_HSITRIM_0                         (0x01U << RCC_CR_HSITRIM_Pos) /*!< 0x00000008 */
#define RCC_CR_HSITRIM_1                         (0x02U << RCC_CR_HSITRIM_Pos) /*!< 0x00000010 */
#define RCC_CR_HSITRIM_2                         (0x04U << RCC_CR_HSITRIM_Pos) /*!< 0x00000020 */
#define RCC_CR_HSITRIM_3                         (0x08U << RCC_CR_HSITRIM_Pos) /*!< 0x00000040 */
#define RCC_CR_HSITRIM_4                         (0x10U << RCC_CR_HSITRIM_Pos) /*!< 0x00000080 */

/*!< ADCPPRE configuration */
#define RCC_CFGR_ADCPRE_Pos                      (14U)                         
#define RCC_CFGR_ADCPRE_Msk                      (0x1U << RCC_CFGR_ADCPRE_Pos) /*!< 0x00004000 */
#define RCC_CFGR_ADCPRE                          RCC_CFGR_ADCPRE_Msk           /*!< ADCPRE bit (ADC prescaler) */

#define RCC_CFGR_ADCPRE_DIV2                     (0x00000000U)                 /*!< PCLK divided by 2 */
#define RCC_CFGR_ADCPRE_DIV4                     (0x00004000U)                 /*!< PCLK divided by 4 */

#define RCC_CFGR_PLLSRC_Pos                      (16U)                         
#define RCC_CFGR_PLLSRC_Msk                      (0x1U << RCC_CFGR_PLLSRC_Pos) /*!< 0x00010000 */
#define RCC_CFGR_PLLSRC                          RCC_CFGR_PLLSRC_Msk           /*!< PLL entry clock source */
#define RCC_CFGR_PLLSRC_HSI_DIV2                 (0x00000000U)                 /*!< HSI clock divided by 2 selected as PLL entry clock source */
#define RCC_CFGR_PLLSRC_HSE_PREDIV               (0x00010000U)                 /*!< HSE/PREDIV clock selected as PLL entry clock source */

/*!< PLLMUL configuration */
#define RCC_CFGR_PLLMUL_Pos                      (18U)                         
#define RCC_CFGR_PLLMUL_Msk                      (0xFU << RCC_CFGR_PLLMUL_Pos) /*!< 0x003C0000 */
#define RCC_CFGR_PLLMUL                          RCC_CFGR_PLLMUL_Msk           /*!< PLLMUL[3:0] bits (PLL multiplication factor) */
#define RCC_CFGR_PLLMUL_0                        (0x1U << RCC_CFGR_PLLMUL_Pos) /*!< 0x00040000 */
#define RCC_CFGR_PLLMUL_1                        (0x2U << RCC_CFGR_PLLMUL_Pos) /*!< 0x00080000 */
#define RCC_CFGR_PLLMUL_2                        (0x4U << RCC_CFGR_PLLMUL_Pos) /*!< 0x00100000 */
#define RCC_CFGR_PLLMUL_3                        (0x8U << RCC_CFGR_PLLMUL_Pos) /*!< 0x00200000 */

#define RCC_CFGR_PLLMUL2                         (0x00000000U)                 /*!< PLL input clock*2 */
#define RCC_CFGR_PLLMUL3                         (0x00040000U)                 /*!< PLL input clock*3 */
#define RCC_CFGR_PLLMUL4                         (0x00080000U)                 /*!< PLL input clock*4 */
#define RCC_CFGR_PLLMUL5                         (0x000C0000U)                 /*!< PLL input clock*5 */
#define RCC_CFGR_PLLMUL6                         (0x00100000U)                 /*!< PLL input clock*6 */
#define RCC_CFGR_PLLMUL7                         (0x00140000U)                 /*!< PLL input clock*7 */
#define RCC_CFGR_PLLMUL8                         (0x00180000U)                 /*!< PLL input clock*8 */
#define RCC_CFGR_PLLMUL9                         (0x001C0000U)                 /*!< PLL input clock*9 */
#define RCC_CFGR_PLLMUL10                        (0x00200000U)                 /*!< PLL input clock10 */
#define RCC_CFGR_PLLMUL11                        (0x00240000U)                 /*!< PLL input clock*11 */
#define RCC_CFGR_PLLMUL12                        (0x00280000U)                 /*!< PLL input clock*12 */
#define RCC_CFGR_PLLMUL13                        (0x002C0000U)                 /*!< PLL input clock*13 */
#define RCC_CFGR_PLLMUL14                        (0x00300000U)                 /*!< PLL input clock*14 */
#define RCC_CFGR_PLLMUL15                        (0x00340000U)                 /*!< PLL input clock*15 */
#define RCC_CFGR_PLLMUL16                        (0x00380000U)                 /*!< PLL input clock*16 */
/*******************  Bit definition for RCC_CFGR2 register  *****************/
/*!< PREDIV configuration */
#define RCC_CFGR2_PREDIV_Pos                     (0U)                          
#define RCC_CFGR2_PREDIV_Msk                     (0xFU << RCC_CFGR2_PREDIV_Pos) /*!< 0x0000000F */
#define RCC_CFGR2_PREDIV                         RCC_CFGR2_PREDIV_Msk          /*!< PREDIV[3:0] bits */
#define RCC_CFGR2_PREDIV_0                       (0x1U << RCC_CFGR2_PREDIV_Pos) /*!< 0x00000001 */
#define RCC_CFGR2_PREDIV_1                       (0x2U << RCC_CFGR2_PREDIV_Pos) /*!< 0x00000002 */
#define RCC_CFGR2_PREDIV_2                       (0x4U << RCC_CFGR2_PREDIV_Pos) /*!< 0x00000004 */
#define RCC_CFGR2_PREDIV_3                       (0x8U << RCC_CFGR2_PREDIV_Pos) /*!< 0x00000008 */

#define RCC_CFGR2_PREDIV_DIV1                    (0x00000000U)                 /*!< PREDIV input clock not divided */
#define RCC_CFGR2_PREDIV_DIV2                    (0x00000001U)                 /*!< PREDIV input clock divided by 2 */
#define RCC_CFGR2_PREDIV_DIV3                    (0x00000002U)                 /*!< PREDIV input clock divided by 3 */
#define RCC_CFGR2_PREDIV_DIV4                    (0x00000003U)                 /*!< PREDIV input clock divided by 4 */
#define RCC_CFGR2_PREDIV_DIV5                    (0x00000004U)                 /*!< PREDIV input clock divided by 5 */
#define RCC_CFGR2_PREDIV_DIV6                    (0x00000005U)                 /*!< PREDIV input clock divided by 6 */
#define RCC_CFGR2_PREDIV_DIV7                    (0x00000006U)                 /*!< PREDIV input clock divided by 7 */
#define RCC_CFGR2_PREDIV_DIV8                    (0x00000007U)                 /*!< PREDIV input clock divided by 8 */
#define RCC_CFGR2_PREDIV_DIV9                    (0x00000008U)                 /*!< PREDIV input clock divided by 9 */
#define RCC_CFGR2_PREDIV_DIV10                   (0x00000009U)                 /*!< PREDIV input clock divided by 10 */
#define RCC_CFGR2_PREDIV_DIV11                   (0x0000000AU)                 /*!< PREDIV input clock divided by 11 */
#define RCC_CFGR2_PREDIV_DIV12                   (0x0000000BU)                 /*!< PREDIV input clock divided by 12 */
#define RCC_CFGR2_PREDIV_DIV13                   (0x0000000CU)                 /*!< PREDIV input clock divided by 13 */
#define RCC_CFGR2_PREDIV_DIV14                   (0x0000000DU)                 /*!< PREDIV input clock divided by 14 */
#define RCC_CFGR2_PREDIV_DIV15                   (0x0000000EU)                 /*!< PREDIV input clock divided by 15 */
#define RCC_CFGR2_PREDIV_DIV16                   (0x0000000FU)                 /*!< PREDIV input clock divided by 16 */


/** @addtogroup Exported_macros
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

/****************************** TSC Instances *********************************/
#define IS_TSC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == TSC)

/*********************** UART Instances : IRDA mode ***************************/
#define IS_IRDA_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/********************* UART Instances : Smard card mode ***********************/
#define IS_SMARTCARD_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/******************** USART Instances : Synchronous mode **********************/
#define IS_USART_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                     ((INSTANCE) == USART2))
                                     
/******************** USART Instances : auto Baud rate detection **************/                                     
#define IS_USART_AUTOBAUDRATE_DETECTION_INSTANCE(INSTANCE) ((INSTANCE) == USART1)
                                                                                              
/******************** UART Instances : Asynchronous mode **********************/
#define IS_UART_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                      ((INSTANCE) == USART2))

/******************** UART Instances : Half-Duplex mode **********************/
#define IS_UART_HALFDUPLEX_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                                 ((INSTANCE) == USART2))

/****************** UART Instances : Hardware Flow control ********************/
#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                           ((INSTANCE) == USART2))

/****************** UART Instances : LIN mode ********************/
#define IS_UART_LIN_INSTANCE(INSTANCE) ((INSTANCE) == USART1)

/****************** UART Instances : wakeup from stop mode ********************/
#define IS_UART_WAKEUP_FROMSTOP_INSTANCE(INSTANCE) ((INSTANCE) == USART1)
/* Old macro definition maintained for legacy purpose */
#define IS_UART_WAKEUP_INSTANCE         IS_UART_WAKEUP_FROMSTOP_INSTANCE

/****************** UART Instances : Driver enable detection ********************/
#define IS_UART_DRIVER_ENABLE_INSTANCE(INSTANCE) (((INSTANCE) == USART1) || \
                                                  ((INSTANCE) == USART2))

/****************************** WWDG Instances ********************************/
#define IS_WWDG_ALL_INSTANCE(INSTANCE)  ((INSTANCE) == WWDG)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F051x8_H */

/**
  * @}
  */

  /**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
