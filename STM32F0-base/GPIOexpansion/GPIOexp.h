#ifndef GPIOEXP_H
#define GPIOEXP_H


#include "stm32f0xx_hal_gpio.h"

#define I2C_TIMEOUT_ADDR    (10000U)
#define I2C_TIMEOUT_BUSY    (10000U)
#define I2C_TIMEOUT_DIR     (10000U)
#define I2C_TIMEOUT_RXNE    (10000U)
#define I2C_TIMEOUT_STOPF   (10000U)
#define I2C_TIMEOUT_TC      (10000U)
#define I2C_TIMEOUT_TCR     (10000U)
#define I2C_TIMEOUT_TXIS    (10000U)
#define I2C_TIMEOUT_FLAG    (10000U)
#define I2C_TIMEOUT_COMMON  (10000U)


typedef enum
{
  I2C_STATE_RESET             = 0x00U,   /*!< Peripheral is not yet Initialized         */
  I2C_STATE_READY             = 0x20U,   /*!< Peripheral Initialized and ready for use  */
  I2C_STATE_BUSY              = 0x24U,   /*!< An internal process is ongoing            */
  I2C_STATE_BUSY_TX           = 0x21U,   /*!< Data Transmission process is ongoing      */
  I2C_STATE_BUSY_RX           = 0x22U,   /*!< Data Reception process is ongoing         */
  I2C_STATE_LISTEN            = 0x28U,   /*!< Address Listen Mode is ongoing            */
  I2C_STATE_BUSY_TX_LISTEN    = 0x29U,   /*!< Address Listen Mode and Data Transmission
                                             process is ongoing                         */
  I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,   /*!< Address Listen Mode and Data Reception
                                             process is ongoing                         */
  I2C_STATE_ABORT             = 0x60U,   /*!< Abort user request ongoing                */
  I2C_STATE_TIMEOUT           = 0xA0U,   /*!< Timeout state                             */
  I2C_STATE_ERROR             = 0xE0U    /*!< Error                                     */

} I2C_StateTypeDef;

namespace GPIOexp
{
    class Controller
    {
    public:
        void init();       
        void loop(uint16_t valvest);
    private:
        void I2C_Master_Transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
        bool HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);
    };

extern GPIOexp::Controller CONTROLLER;

}
#endif // CONTROLLER_H
