import qbs

Product
{
    name: "RCservor_controller"
//    type: ["application", "hex", "flash"]
    type: ["application", "hex"]

    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "timer"}
    Depends{name: "GPIO"}
//    Depends{name: "GPIOexp"}
    Depends{name: "uart"}


    cpp.includePaths: [project.sourceDirectory]
    cpp.defines:
    [
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:["main.cpp","controller.h","command.h","command.cpp"]
}

