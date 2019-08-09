import qbs

Project
{
    name: "stm32-core"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "D:/QT/Gcc/7 2018-q2-update/bin/"
    property string openOCDExe: "D:/QT/OpenOCD-20180728/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "D:/QT/OpenOCD-20180728/OpenOCD-20180728/share/openocd"
    references:[
        "modules/stm32f0/mdk.qbs",

        /*LIBRARIES*/
        "uart/stm32.qbs",

        /*APPLICATION*/
        //"adc/stm32.qbs",
        //"adc-dma/stm32.qbs",
        //"blink/stm32.qbs",
        //"spi/stm32.qbs",
        //"spi-hardware/stm32.qbs",
        //"encoder-timer2/stm32.qbs",
        //"pwm-timer1-6ch/stm32.qbs",
        "motor-controller/stm32.qbs",
        //"motor-controller2/stm32.qbs",
    ]
}
