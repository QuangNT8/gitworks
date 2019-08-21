import qbs

Product
{
    name: "controller"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]
    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}
    Depends{name: "motor"}
    Depends{name: "valve"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "main.cpp",
        "controller.h",
        "controller.cpp",
    ]
}

