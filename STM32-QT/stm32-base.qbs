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
    //property string driverPath: "E:/gitworks/nRF5/"
    property string nrfjprogPath: "nrfjprog"
}
