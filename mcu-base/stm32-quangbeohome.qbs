import qbs

Project
{
    name: "stm32-core"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "E:/QuangTRHN/Setup/GCC/gcc-arm-none-eabi/bin/"
    property string openOCDExe: "E:/QuangTRHN/Setup/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "E:/QuangTRHN/Setup/OpenOCD-20180728/share/openocd"
    references:[
        "modules/stm32f0/mdk.qbs",

        /*LIBRARIES*/
        "uart/stm32.qbs",

        /*APPLICATION*/
        "blink/stm32.qbs",
        //"motor-controller/stm32.qbs",
    ]
}
