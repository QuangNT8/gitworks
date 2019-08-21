import qbs

Product
{
    name: "encoder-timer2"
    //type: ["application", "hex", "flash"]
    //type: ["application", "hex"]

    Depends {name: "cpp"}
    Depends {name: "stm32f0"}
    Depends {name: "mdk"}
    Depends {name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "encoder.h",
        "stm32f0.cpp",
        "main.cpp",
        "command.cpp",
        "command.h",
    ]
}


