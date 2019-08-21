#ifndef TIMER_H
#define TIMER_H

#include "stm32f051x8.h"
#include "stm32f0xx_hal_dma.h"
#include "stm32f0xx_hal_tim.h"

/* AF 2 */
#define GPIO_AF2_TIM1         ((uint8_t)0x02U)  /*!< AF2: TIM1 Alternate Function mapping      */
#define GPIO_AF2_TIM2         ((uint8_t)0x02U)  /*!< AF2: TIM2 Alternate Function mapping      */
#define GPIO_AF2_TIM16        ((uint8_t)0x02U)  /*!< AF2: TIM16 Alternate Function mapping     */
#define GPIO_AF2_TIM17        ((uint8_t)0x02U)  /*!< AF2: TIM17 Alternate Function mapping     */
#define GPIO_AF2_EVENTOUT     ((uint8_t)0x02U)  /*!< AF2: EVENTOUT Alternate Function mapping  */

typedef void (* stm_timer_event_handler_t)(uint32_t event_type);


namespace timer
{
    class Controller
    {
    public:
        void init(uint32_t tim);
        void init_IT(stm_timer_event_handler_t func_prt);
        void loop();
    private:
        static void MX_TIMx_Init(TIM_TypeDef *TIM);
        static void MX_TIM2_Init(void);
        static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
        static void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

    };

extern timer::Controller CONTROLLER;

}
#endif // CONTROLLER_H
