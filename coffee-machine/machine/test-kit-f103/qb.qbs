import qbs

Project
{
    name: "test-f103"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "C:/qb/gcc-arm-none-eabi/bin/"
    property string openOCDExe: "C:/qb/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "C:/qb/OpenOCD-20180728/share/openocd"
    references:[
        "modules/stm32f1/mdk.qbs",
        /*LIBRARIES*/
        "uart/stm32.qbs",
        "uart2/stm32.qbs",
        /*APPLICATION*/
        "test-f103/stm32.qbs",
        "valve/stm32f1.qbs",
    ]
}
