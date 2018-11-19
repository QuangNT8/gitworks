import qbs

Project
{
    minimumQbsVersion: "1.7.1"
    references:
    [
        //"modules/nrf52/nrf52-sdk.qbs",
        //"blinky/blinky.qbs",
        //"radio/TX/radioTX.qbs",
        //"radio/RX/radioRX.qbs",
        //"UART_TEST/UART_TEST.qbs",
        //"ADC_TEST/ADC_TEST.qbs",
        "Flash_TEST/FLASH_TEST.qbs",
    ]

    property string gccPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/"
    property string linkerPath: "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3/bin/arm-none-eabi-g++"
    property string sdkPath: "E:/gitworks/nRF5/"
    property string nrfjprogPath: "nrfjprog"
}
