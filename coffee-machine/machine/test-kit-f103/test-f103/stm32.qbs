import qbs

Product
{
    name: "test-f103"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]
    Depends{name: "cpp"}
    Depends{name: "stm32f1"}
    Depends{name: "mdk"}
    Depends{name: "uart"}
    Depends{name: "uart2"}
    Depends{name: "valve"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "cmdscale.cpp",
        "controller.cpp",
        "controller.h",
        "gpio.cpp",
        "main.cpp",
        "command.h",
        "command.cpp",
        "system_clock.cpp",
    ]
}

