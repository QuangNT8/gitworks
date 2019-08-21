import qbs 1.0

StaticLibrary
{
    name: "ADC"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    Depends{name: "uart"}
    cpp.defines:
    [
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:
    [
        "adc.h",
        "adc.cpp",
    ]
}


