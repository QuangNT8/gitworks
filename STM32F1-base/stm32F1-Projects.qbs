import qbs

Project
{
    name: "stm32f1-projects"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"

    property string openOCDExe: "D:/uki/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "D:/uki/OpenOCD-20180728/share/openocd"
    references:[
//        "modules/stm32f1/mdk.qbs",
        "CubeGen/mdk.qbs",
        /* LIBRARIES */
        "uart/stm32.qbs",
//        "Stm32_USART/Usart.qbs",
        "blink/stm32.qbs",
//        "CAN/can.qbs",
//        "stm32_GPIO/gpio.qbs",
    ]
}
