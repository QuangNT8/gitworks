import qbs 1.0

StaticLibrary
{
    name: "PWM"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    cpp.defines:
    [
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "PWM.h",
        "PWM.cpp",
    ]
}


