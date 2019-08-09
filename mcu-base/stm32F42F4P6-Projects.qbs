import qbs

Project
{
    name: "stm32f042F-projects"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"

    property string openOCDExe: "D:/uki/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "D:/uki/OpenOCD-20180728/share/openocd"
    references:[
        "modules/stm32f0/mdk.qbs",
        /*LIBRARIES*/
        "uart/stm32.qbs",
//        "Stm32_USART/Usart.qbs",
        "blink/stm32.qbs",
//        "Stm32_System_clock/osc.qbs",
//        "stm32_timer/timer.qbs",
//        "PWM/PWM.qbs",
//        "STM32ADC/adc.qbs",
//        "STM32SPI/spi.qbs",
//        "MCP_CAN/mcp_can.qbs",
        "stm32_GPIO/gpio.qbs",
//        "RCservoLib/RCservo.qbs",
//        "RCservor_controller/RCservor_controller.qbs",
//        "Led_driver/Led_driver.qbs",
//        "motor-coffee-machine/stm32.qbs",
//        "GPIOexpansion/GPIOexp.qbs",
    ]
}
