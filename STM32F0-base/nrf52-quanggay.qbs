import qbs

Project
{
    minimumQbsVersion: "1.6.0"
    name: "nrf52-core"
    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"
    property string linkerPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/arm-none-eabi-g++"
    property string sdkPath: "D:/gitworks/nRF5/"
    property string nrfjprogPath: "nrfjprog"

    references:
    [
        "modules/nrf52/mdk.qbs",
        "uart/nrf52.qbs",
        "timer/timer.qbs",
        "SSADC/sadc.qbs",
        "blink/nrf52.qbs",
        "AcMesurements/AcMesurement.qbs",
        "radio/radio.qbs",
    ]
}
