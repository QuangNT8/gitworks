import qbs 1.0

StaticLibrary
{
    name: "CAN"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    Depends { name:"mdk"}
    Depends{name: "uart"}

    cpp.defines:
    [
        'STM32F042x6',
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "can.h",
        "can.cpp",
    ]
}


