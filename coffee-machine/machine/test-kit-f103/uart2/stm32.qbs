import qbs 1.0

StaticLibrary
{
    name: "uart2"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    Depends { name:"mdk"}

    files:
    [
        "controller.h",
        "controller.cpp",
        "controller.h",
        "stm32f1.cpp",
    ]
}


