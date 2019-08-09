import qbs

Product
{
    name: "adc"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]

    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:["main.cpp", "controller.h", "stm32f0.cpp", "command.cpp", "command.h"]
}
