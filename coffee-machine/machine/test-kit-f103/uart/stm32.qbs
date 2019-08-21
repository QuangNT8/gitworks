import qbs 1.0

StaticLibrary
{
    name: "uart"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    Depends { name:"mdk"}

    files:
    [
        "controller.h",
        "controller.cpp",
        "stm32f1.cpp",
        "oscilloscope.h",
    ]
}


