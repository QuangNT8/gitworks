import qbs

Project
{
    minimumQbsVersion: "1.6.0"
    name: "nrf52-core"
    property string linkerPath: "/home/hongsan/nrf52/arm-none-eabi-ld"
    property string gccPath: ""
    property string nrfjprogPath: "/home/hongsan/nrf52/nrfjprog/nrfjprog"

    references:
    [
        "modules/nrf52/mdk.qbs",
        "uart/nrf52.qbs",
        "blink/nrf52.qbs",
        "motor-controller/nrf52.qbs",
    ]
}
