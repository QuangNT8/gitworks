import qbs

Product
{
    name: "blink"
    type: ["application", "hex", "flash"]
//    type: ["application", "hex"]

    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "GPIO"}

    cpp.includePaths: [project.sourceDirectory]
    cpp.defines:
    [
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:["main.cpp","stm32.cpp","controller.h"]
}

