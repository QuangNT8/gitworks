import qbs 1.0

StaticLibrary
{
    name: "CAN"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    Depends { name:"mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]

    cpp.defines:
    [
        'STM32F103xB',
        '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "can.h",
        "can.cpp",
    ]
}


