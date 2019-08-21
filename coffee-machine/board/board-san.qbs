import qbs

Project
{
    name: "bj-board"
    minimumQbsVersion: "1.6.0"
    property string gccPath: ""
    property string openOCDExe: "openocd"
    property string openOCDPath: "/usr/share/openocd"
    references:
    [
        "modules/stm32f0/mdk.qbs",
        "uart/stm32.qbs",
        "motor/stm32.qbs",
        "controller/stm32.qbs",
        "valve/stm32.qbs",
    ]
}
