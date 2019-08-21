import qbs 1.0

StaticLibrary
{
    name: "uart"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}
    Depends { name:"mdk"}

    files:
    [
        "controller.h",
        "controller.cpp",
        "nrf52.cpp",
        "oscilloscope.h"
    ]
}


