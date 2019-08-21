import qbs

Product
{
    name: "Led_driver"
    type: ["application", "hex", "flash"]
//    type: ["application", "hex"]

    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
//    Depends{name: "timer"}
    Depends{name: "PWM"}
    Depends{name: "ADC"}
    Depends{name: "GPIO"}
//    Depends{name: "GPIOexp"}
    Depends{name: "uart"}
    Depends{name: "SPI"}
    Depends{name: "mcp_can"}

    cpp.includePaths: [project.sourceDirectory]
    cpp.defines:
    [
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:["main.cpp","command.h","command.cpp"]
}

