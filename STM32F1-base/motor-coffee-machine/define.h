#ifndef DEFINE_H
#define DEFINE_H

#include <stdint.h>

/*ADC Calibrate*/
/*
 * VCC ACS712(5A) = 4.45V, VCC STM = 3V
 * At 0A output ACS712 = 4.45/2 = 2.225V <=> ADC(2.225V) =  (4094*2.225V)/3V = 3036
*/
#define CALIBRATE_ADC       3036
#define AMPE_ZERO           2225//mV
#define ACS712_5A_MV_A      185//mV do nhay cam dau ra 185mV/A

/*GPIO*/
//#define LED_ON              (GPIOC->BSRR |= GPIO_BSRR_BR_13)
//#define LED_OFF             (GPIOC->BSRR |= GPIO_BSRR_BS_13)
//#define LED_TOGGLE          (GPIOC->ODR ^= GPIOC->ODR ^= GPIO_ODR_13)
#define LED_ON              (GPIOC->BSRR |= GPIO_BSRR_BR_9)
#define LED_OFF             (GPIOC->BSRR |= GPIO_BSRR_BS_9)
#define LED_TOGGLE          (GPIOC->ODR ^= GPIO_ODR_9)

#define ALOW_ON             (GPIOA->BSRR |= GPIO_BSRR_BS_9)
#define ALOW_OFF            (GPIOA->BSRR |= GPIO_BSRR_BR_9)
#define BLOW_ON             (GPIOB->BSRR |= GPIO_BSRR_BS_4)
#define BLOW_OFF            (GPIOB->BSRR |= GPIO_BSRR_BR_4)

/*AS5047*/
#define CLEAR_SCS_AS5047    (GPIOA->BSRR |= GPIO_BSRR_BR_2)
#define SET_SCS_AS5047      (GPIOA->BSRR |= GPIO_BSRR_BS_2)

#define OP_WRITE 0x0000
#define OP_READ  0x4000

#define PARITY_EVEN 0x0000
#define PARITY_ODD  0x8000

#define ADDR_NOP          0x0000
#define ADDR_ERRFL        0x0001
#define ADDR_PROG         0x0003
#define ADDR_DIAAGC       0x3FFC
#define ADDR_MAG          0x3FFD
#define ADDR_ANGLEUNC     0x3FFE
#define ADDR_ANGLECOM     0x3FFF
#define ADDR_SETTINGS1    0x0018
#define ADDR_SETTINGS2    0x0019

#define CMD_R_NOP       (PARITY_ODD  | OP_READ | ADDR_NOP)
#define CMD_R_ERRFL     (PARITY_EVEN | OP_READ | ADDR_ERRFL)
#define CMD_R_PROG      (PARITY_ODD  | OP_READ | ADDR_PROG)
#define CMD_R_DIAAGC    (PARITY_ODD  | OP_READ | ADDR_DIAAGC)
#define CMD_R_MAG       (PARITY_EVEN | OP_READ | ADDR_MAG)
#define CMD_R_ANGLEUNC  (PARITY_EVEN | OP_READ | ADDR_ANGLEUNC)
#define CMD_R_ANGLECOM  (PARITY_ODD  | OP_READ | ADDR_ANGLECOM)
#define CMD_R_SETTINGS1 (PARITY_ODD  | OP_READ | ADDR_SETTINGS1)
#define CMD_R_SETTINGS2 (PARITY_EVEN | OP_READ | ADDR_SETTINGS2)

/*Queue*/
#define CMD_BUF_SIZE 64

/*Sincos*/
//#define PULSE_PI_2 8
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 59, 115, 167, 212, 249, 277, 294, 300};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 78, 153, 222, 283, 333, 370, 392, 400};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 98, 191, 278, 354, 416, 462, 490, 500};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 117, 230, 333, 424, 499, 554, 588, 600};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 137, 268, 389, 495, 582, 647, 687, 700};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 156, 306, 444, 566, 665, 739, 785, 800};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 176, 344, 500, 636, 748, 831, 883, 900};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 195, 383, 556, 707, 831, 924, 981, 1000};

#define PULSE_PI_2 16
static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 29, 59, 87, 115, 141, 167, 190, 212, 232, 249, 265, 277, 287, 294, 299, 300};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 39, 78, 116, 153, 189, 222, 254, 283, 309, 333, 353, 370, 383, 392, 398, 400};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 49, 98, 145, 191, 236, 278, 317, 354, 387, 416, 441, 462, 478, 490, 498, 500};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 69, 137, 203, 268, 330, 389, 444, 495, 541, 582, 617, 647, 670, 687, 697, 700};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 88, 175, 261, 344, 424, 500, 570, 636, 695, 748, 793, 831, 861, 882, 895, 900};

//#define PULSE_PI_2 64
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 7, 15, 22, 29, 37, 44, 51, 59, 66, 73, 80, 87, 94, 101, 108, 115, 122, 128, 135, 141, 148, 154, 160, 167, 173, 179, 185, 190, 196, 201, 207, 212, 217, 222, 227, 232, 237, 241, 245, 249, 253, 257, 261, 265, 268, 271, 274, 277, 280, 282, 285, 287, 289, 291, 293, 294, 296, 297, 298, 299, 299, 300, 300, 300};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 10, 20, 29, 39, 49, 59, 68, 78, 88, 97, 107, 116, 125, 135, 144, 153, 162, 171, 180, 189, 197, 206, 214, 222, 230, 238, 246, 254, 261, 269, 276, 283, 290, 296, 303, 309, 315, 321, 327, 333, 338, 343, 348, 353, 357, 362, 366, 370, 373, 377, 380, 383, 386, 388, 390, 392, 394, 396, 397, 398, 399, 400, 400, 400};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 15, 29, 44, 59, 73, 88, 103, 117, 131, 146, 160, 174, 188, 202, 216, 230, 243, 257, 270, 283, 296, 308, 321, 333, 345, 357, 369, 381, 392, 403, 414, 424, 435, 445, 454, 464, 473, 482, 491, 499, 507, 515, 522, 529, 536, 542, 549, 554, 560, 565, 570, 574, 578, 582, 585, 588, 591, 594, 595, 597, 598, 599, 600, 600};
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 20, 39, 59, 78, 98, 117, 137, 156, 175, 194, 213, 232, 251, 270, 288, 306, 324, 342, 360, 377, 394, 411, 428, 444, 461, 477, 492, 508, 523, 537, 552, 566, 579, 593, 606, 618, 631, 643, 654, 665, 676, 686, 696, 706, 715, 723, 731, 739, 746, 753, 760, 766, 771, 776, 781, 785, 788, 791, 794, 796, 798, 799, 800, 800};

//#define PULSE_PI_2 128
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 4, 7, 11, 15, 18, 22, 26, 29, 33, 37, 40, 44, 48, 51, 55, 59, 62, 66, 69, 73, 76, 80, 84, 87, 91, 94, 98, 101, 105, 108, 111, 115, 118, 122, 125, 128, 132, 135, 138, 141, 145, 148, 151, 154, 157, 160, 164, 167, 170, 173, 176, 179, 182, 185, 187, 190, 193, 196, 199, 201, 204, 207, 210, 212, 215, 217, 220, 222, 225, 227, 230, 232, 234, 237, 239, 241, 243, 245, 247, 249, 251, 253, 255, 257, 259, 261, 263, 265, 266, 268, 270, 271, 273, 274, 276, 277, 279, 280, 281, 282, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 293, 294, 295, 296, 296, 297, 297, 298, 298, 299, 299, 299, 299, 300, 300, 300, 300, 300};

//#define PULSE_PI_2 256
//static const uint16_t SINCOS[PULSE_PI_2+1] = {0, 2, 4, 6, 7, 9, 11, 13, 15, 17, 18, 20, 22, 24, 26, 28, 29, 31, 33, 35, 37, 39, 40, 42, 44, 46, 48, 49, 51, 53, 55, 57, 59, 60, 62, 64, 66, 68, 69, 71, 73, 75, 76, 78, 80, 82, 84, 85, 87, 89, 91, 92, 94, 96, 98, 99, 101, 103, 105, 106, 108, 110, 111, 113, 115, 117, 118, 120, 122, 123, 125, 127, 128, 130, 132, 133, 135, 137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 153, 154, 156, 157, 159, 160, 162, 164, 165, 167, 168, 170, 171, 173, 174, 176, 177, 179, 180, 182, 183, 185, 186, 187, 189, 190, 192, 193, 195, 196, 197, 199, 200, 201, 203, 204, 206, 207, 208, 210, 211, 212, 213, 215, 216, 217, 219, 220, 221, 222, 224, 225, 226, 227, 228, 230, 231, 232, 233, 234, 235, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 265, 266, 267, 268, 269, 270, 270, 271, 272, 273, 274, 274, 275, 276, 276, 277, 278, 279, 279, 280, 281, 281, 282, 282, 283, 284, 284, 285, 285, 286, 287, 287, 288, 288, 289, 289, 290, 290, 291, 291, 291, 292, 292, 293, 293, 293, 294, 294, 295, 295, 295, 296, 296, 296, 296, 297, 297, 297, 298, 298, 298, 298, 298, 299, 299, 299, 299, 299, 299, 299, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300};

/*Speed*/
#define CLIBRATE_SPEED ((200 * PULSE_PI_2)/60)//https://docs.google.com/spreadsheets/d/1NWE0vmMptKXrapdPtWRU9O4l6lhB9SgsXJ7o_Plxj6w/edit#gid=0

#endif // DEFINE_H