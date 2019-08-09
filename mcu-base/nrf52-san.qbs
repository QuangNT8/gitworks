import qbs

Project
{
    minimumQbsVersion: "1.6.0"
    name: "nrf52-core"
    property string linkerPath: "/home/hongsan/nrf52/arm-none-eabi-ld"
    property string gccPath: ""
    property string nrfjprogPath: "/home/hongsan/nrf52/nrfjprog/nrfjprog"

//    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"
//    property string linkerPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/arm-none-eabi-g++"
//    property string sdkPath: "E:/gitworks/nRF5/"
//    property string nrfjprogPath: "nrfjprog"

//	property string linkerPath : "/home/hongsan/nrf52/gcc/bin/arm-none-eabi-myld"
//	property string nrfjprogPath: "path to nrfjprog"
//	property string gccPath: ""

    references:
    [
        "modules/nrf52/mdk.qbs",
        "uart/nrf52.qbs",
        //"blink/nrf52.qbs",
        //"motor-controller/nrf52.qbs",
        "testosc/nrf52.qbs",
    ]
}
