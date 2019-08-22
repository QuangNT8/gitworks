import qbs

Product
{
    name: "blink"
    type: ["application", "hex", "flash"]
//    type: ["application", "hex"]

    Depends{name: "cpp"}
    Depends{name: "stm32f1"}
    Depends{name: "mdk"}
    Depends{name: "uart"}
//    Depends{name: "CAN"}

    cpp.includePaths: [project.sourceDirectory]
    cpp.defines:
    [
       'STM32F103xB',
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:["main.cpp","command.h","command.cpp"]
}

