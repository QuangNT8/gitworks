import qbs 1.0

StaticLibrary
{
    name: "Usart"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    Depends { name:"mdk"}
    cpp.defines:
    [
        'STM32F103xB',
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "Usart.h",
        "Usart.cpp",
    ]
}


