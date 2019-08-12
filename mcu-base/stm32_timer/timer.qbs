import qbs 1.0

StaticLibrary
{
    name: "timer"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    cpp.defines:
    [
        'STM32F042x6',
        '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "timer.h",
        "timer.cpp",
    ]
}

