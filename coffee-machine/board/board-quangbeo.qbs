import qbs

Project
{
    name: "bj-board"
    minimumQbsVersion: "1.6.0"
    property string gccPath: "C:/qb/gcc-arm-none-eabi/bin/"
    property string openOCDExe: "C:/qb/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "C:/qb/OpenOCD-20180728/share/openocd"
    references:[
        "modules/stm32f0/mdk.qbs",
        "uart/stm32.qbs",
        "motor/stm32.qbs",
        "controller/stm32.qbs",
        "valve/stm32.qbs",
    ]
}
