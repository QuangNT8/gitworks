import qbs 1.0

StaticLibrary
{
    name: "timer"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}

    files:
    [
        "timer.h",
        "timer.cpp",
    ]
}


