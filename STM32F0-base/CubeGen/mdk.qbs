import qbs 1.0

StaticLibrary
{
    name: "mdk"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    cpp.defines:
    [
        'STM32F051x8',
       '___int16_t_defined',
        '___int8_t_defined',
    ]

    cpp.includePaths:
    [
        "Drivers/STM32F0xx_HAL_Driver/Inc/",
        "Drivers/CMSIS/Device/ST/STM32F0xx/Include/",
        "Drivers/CMSIS/Include/",
        "Inc/",
    ]

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:
        [
            "Drivers/STM32F0xx_HAL_Driver/Inc/",
            "Drivers/CMSIS/Device/ST/STM32F0xx/Include/",
            "Drivers/CMSIS/Include/",
            "Inc/",
            path,
        ]
    }

    files:
    [
        "system/cmsis/inc/*.h",
        "Drivers/STM32F0xx_HAL_Driver/Inc/*.h",
        "Drivers/STM32F0xx_HAL_Driver/Src/*.c",
        "Drivers/CMSIS/Device/ST/STM32F0xx/Include/*.h",
        "Drivers/CMSIS/Include/*.h",
        "startup/startup_stm32f051x8.s",
        "Src/system_stm32f0xx.c",
        "Inc/*.h",
    ]
}


