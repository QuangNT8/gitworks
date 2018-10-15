import qbs

Product
{
    name: "blink"
    //type: ["application", "hex", "flash"]
    type: ["application", "hex"]

    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "timer"}
    Depends{name: "mdk"}

    cpp.includePaths: [project.sourceDirectory]

    files:["main.cpp",]
}

