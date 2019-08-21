#ifndef RADIO_H
#define RADIO_H
#include "nrf.h"

#define PACKET_BASE_ADDRESS_LENGTH  (4UL)                   //!< Packet base address length field size in bytes
#define PACKET_STATIC_LENGTH        (1UL)                   //!< Packet static length in bytes
#define PACKET_PAYLOAD_MAXSIZE      (PACKET_STATIC_LENGTH)  //!< Packet payload maximum size in bytes

/* These are set to zero as ShockBurst packets don't have corresponding fields. */
#define PACKET_S1_FIELD_SIZE      (0UL)  /**< Packet S1 field size in bits. */
#define PACKET_S0_FIELD_SIZE      (0UL)  /**< Packet S0 field size in bits. */
#define PACKET_LENGTH_FIELD_SIZE  (0UL)  /**< Packet length field size in bits. */

namespace radio
{
    class Controller
    {
    public:
        void init(uint32_t *packet);
        void loop();
        void send_packet();
    private:
        static uint32_t swap_bits(uint32_t inp);
        static uint32_t bytewise_bitswap(uint32_t inp);
        void radio_configure();
        void clock_initialization();
    };

extern radio::Controller CONTROLLER;

}
#endif // CONTROLLER_H
