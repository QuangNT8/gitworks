import qbs 1.0

StaticLibrary
{
    name: "timer"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}
    Depends { name:"mdk"}

    files:
    [
        "timer.h",
        "timer.cpp",
        "nrf_drv_timer.h",
    ]
}


