import qbs

Product
{
    name: "motor-controller2"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]
    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:["controller.h", "controller.cpp", "stm32f0.cpp", "main.cpp", "command.cpp", "command.h", "define.h",
        //"*.cpp"
    ]
}

