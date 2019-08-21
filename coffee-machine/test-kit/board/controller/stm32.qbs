import qbs

Product
{
    name: "controller"
    type: ["application", "hex", "flash"] //for deploy board
    //type: ["application", "hex"] //for build
    Depends{name: "cpp"}
    Depends{name: "stm32f1"}
    Depends{name: "mdk"}
    Depends{name: "kit"}
    Depends{name: "uart"}
    Depends{name: "uart2"}
    Depends{name: "valve"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "command.cpp",
        "command.h",
        "event.cpp",
        "main.cpp",
    ]
}

