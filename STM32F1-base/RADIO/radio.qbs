import qbs 1.0

StaticLibrary
{
    name: "radio"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}
    Depends { name:"mdk"}

    files:
    [
        "radio.h",
        "radio.cpp",
    ]
}


