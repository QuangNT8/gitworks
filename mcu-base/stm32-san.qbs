import qbs

Project
{
    name: "stm32-core"
    minimumQbsVersion: "1.6.0"
    property string gccPath: ""
    property string openOCDExe: "openocd"
    property string openOCDPath: "/usr/share/openocd"
    references:[
        "modules/stm32f0/mdk.qbs",
        "uart/stm32.qbs",
        //"blink/stm32.qbs",
        //"motor-controller/stm32.qbs",
        "testosc/stm32.qbs",
    ]
}
