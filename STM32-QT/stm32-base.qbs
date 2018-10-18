import qbs

Project
{
    minimumQbsVersion: "1.7.1"
    references:
    [
        //"modules/nrf52/nrf52-sdk.qbs",
        "powersupply/power.qbs",
    ]

    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"
    property string linkerPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/arm-none-eabi-g++"
    property string openOCDExe: "E:/uki/OpenOCD-20180728/bin/openocd.exe"
    property string openOCDPath: "E:/uki/OpenOCD-20180728/share/openocd"
    property string nrfjprogPath: "nrfjprog"
}
