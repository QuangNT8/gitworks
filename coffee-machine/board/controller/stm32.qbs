import qbs

Product
{
    name: "controller"
    type: ["application", "hex", "flash"] //for deploy board
    //type: ["application", "hex"] //for build
    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}
    Depends{name: "motor"}
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

