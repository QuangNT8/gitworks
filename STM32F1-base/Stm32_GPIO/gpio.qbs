import qbs 1.0

StaticLibrary
{
    name: "GPIO"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    cpp.defines:
    [
        'STM32F103xB',
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "GPIO.h",
        "GPIO.cpp",
    ]
}


