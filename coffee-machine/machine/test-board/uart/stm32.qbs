import qbs 1.0

StaticLibrary
{
    name: "uart"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    files:
    [
        "controller.h",
        "controller.cpp",
        "stm32f0.cpp",
        "oscilloscope.h"
    ]
}


