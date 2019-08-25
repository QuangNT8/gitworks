import qbs 1.0

StaticLibrary
{
    name: "Usart"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    cpp.defines:
    [
        'STM32F051x8',
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "Usart.h",
        "Usart.cpp",
    ]
}

