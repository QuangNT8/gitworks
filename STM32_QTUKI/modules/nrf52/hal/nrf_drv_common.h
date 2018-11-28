/**
 * Copyright (c) 2015 - 2017, Nordic Semiconductor ASA
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 * 
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 * 
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 * 
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 * 
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
#ifndef NRF_DRV_COMMON_H__
#define NRF_DRV_COMMON_H__

#include <stdint.h>
#include <stdbool.h>
#include "nrf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Offset of event registers in every peripheral instance.
 *
 * This is the offset where event registers start in  every peripheral.
 */
#define NRF_DRV_COMMON_EVREGS_OFFSET 0x100U


/**
 * @brief Driver state.
 */
typedef enum
{
    NRF_DRV_STATE_UNINITIALIZED, /**< Uninitialized. */
    NRF_DRV_STATE_INITIALIZED, /**< Initialized but powered off. */
    NRF_DRV_STATE_POWERED_ON
} nrf_drv_state_t;

/**
 * @brief Driver power state selection.
 */
typedef enum
{
    NRF_DRV_PWR_CTRL_ON,   /**< Power on request. */
    NRF_DRV_PWR_CTRL_OFF   /**< Power off request. */
} nrf_drv_pwr_ctrl_t;

/**
 * @brief IRQ handler type.
 */
typedef void (*nrf_drv_irq_handler_t)(void);





/**
 * @brief Check if interrupt is enabled
 *
 * Function that checks if selected interrupt is enabled.
 *
 * @param[in] IRQn     Interrupt id
 *
 * @retval true  Selected IRQ is enabled.
 * @retval false Selected IRQ is disabled.
 */
__STATIC_INLINE bool nrf_drv_common_irq_enable_check(IRQn_Type IRQn);

/**
 * @brief Function disables NVIC interrupt
 *
 * @param[in] IRQn     Interrupt id
 */
__STATIC_INLINE void nrf_drv_common_irq_disable(IRQn_Type IRQn);

/**
 * @brief Convert bit position to event code
 *
 * Function for converting the bit position in INTEN register to event code
 * that is equivalent to the offset of the event register from the beginning
 * of peripheral instance.
 *
 * For example the result of this function can be casted directly to
 * the types like @ref nrf_twis_event_t or @ref nrf_rng_event_t
 *
 * @param bit Bit position in INTEN register
 * @return Event code to be casted to the right enum type or to be used in functions like
 * @ref nrf_rng_event_get
 *
 * @sa nrf_drv_event_to_bitpos
 */
__STATIC_INLINE uint32_t nrf_drv_bitpos_to_event(uint32_t bit);

/**
 * @brief Convert event code to bit position
 *
 * This function can be used to get bit position in INTEN register from event code.
 *
 * @param event Event code that may be casted from enum values from types like
 * @ref nrf_twis_event_t or @ref nrf_rng_event_t
 * @return Bit position in INTEN register that corresponds to the given code.
 *
 * @sa nrf_drv_bitpos_to_event
 */
__STATIC_INLINE uint32_t nrf_drv_event_to_bitpos(uint32_t event);

/**
 * @brief Get interrupt number connected with given instance
 *
 * Function returns interrupt number for a given instance of any peripheral.
 * @param[in] pinst Pointer to peripheral registry
 * @return Interrupt number
 */
__STATIC_INLINE IRQn_Type nrf_drv_get_IRQn(void const * const pinst);



/**
 * @brief Check if given object is in RAM
 *
 * Function for analyzing if given location is placed in RAM.
 * This function is used to determine if we have address that can be supported by EasyDMA.
 * @param[in] ptr Pointer to the object
 * @retval true  Object is located in RAM
 * @retval false Object is not located in RAM
 */
__STATIC_INLINE bool nrf_drv_is_in_RAM(void const * const ptr);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE bool nrf_drv_common_irq_enable_check(IRQn_Type IRQn)
{
    return 0 != (NVIC->ISER[(((uint32_t)(int32_t)IRQn) >> 5UL)] &
        (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL)));
}

__STATIC_INLINE void nrf_drv_common_irq_disable(IRQn_Type IRQn)
{
    NVIC_DisableIRQ(IRQn);
}

__STATIC_INLINE uint32_t nrf_drv_bitpos_to_event(uint32_t bit)
{
    return NRF_DRV_COMMON_EVREGS_OFFSET + bit * sizeof(uint32_t);
}

__STATIC_INLINE uint32_t nrf_drv_event_to_bitpos(uint32_t event)
{
    return (event - NRF_DRV_COMMON_EVREGS_OFFSET) / sizeof(uint32_t);
}

__STATIC_INLINE IRQn_Type nrf_drv_get_IRQn(void const * const pinst)
{
    uint8_t ret = (uint8_t)((uint32_t)pinst>>12U);
    return (IRQn_Type) ret;
}


__STATIC_INLINE bool nrf_drv_is_in_RAM(void const * const ptr)
{
    return ((((uintptr_t)ptr) & 0xE0000000u) == 0x20000000u);
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION


#ifdef __cplusplus
}
#endif

#endif // NRF_DRV_COMMON_H__

/** @} */