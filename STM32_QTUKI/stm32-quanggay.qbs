import qbs

Project
{
    name: "stm32-core"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"

    property string openOCDExe: "D:/QT/OpenOCD-20180728/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "D:/QT/OpenOCD-20180728/OpenOCD-20180728/share/openocd"
    references:[
        "modules/stm32f0/mdk.qbs",
        /*LIBRARIES*/
        //"uart/stm32.qbs",
        "blink/stm32.qbs",
        "stm32_timer/timer.qbs",

    ]
}
