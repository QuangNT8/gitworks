import qbs

Product
{
    name: "pwm-timer1-6ch"
    //type: ["application", "hex", "flash"]
    //type: ["application", "hex"]

    Depends {name: "cpp"}
    Depends {name: "stm32f0"}
    Depends {name: "mdk"}
    Depends {name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "pwm.h",
        "stm32f0.cpp",
        "main.cpp",
        "command.cpp",
        "command.h",
    ]
}


