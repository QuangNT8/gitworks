import qbs 1.0

StaticLibrary
{
    name: "AcMesurement"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}
    Depends { name:"mdk"}
    Depends { name:"timer"}
    Depends{name: "sadc"}
    Depends{name: "uart"}
    files:
    [
        "AcMesurement.h",
        "AcMesurement.cpp",
    ]
}


