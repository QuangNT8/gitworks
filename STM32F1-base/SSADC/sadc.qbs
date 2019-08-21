import qbs 1.0

StaticLibrary
{
    name: "sadc"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}
    Depends { name:"mdk"}

    files:
    [
        "sadc.h",
        "sadc.cpp",
    ]
}


