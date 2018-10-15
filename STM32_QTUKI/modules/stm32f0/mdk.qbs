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
        "system/include/",
        "system/cmsis/inc/",
        "system/STM32F0xx_HAL_Driver/Inc/",
        "system/STM32F0xx_HAL_Driver/Src/",
        "system/cmsis/inc",

    ]

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:
        [
            "system/include/",
            "system/cmsis/inc/",
            "system/STM32F0xx_HAL_Driver/Inc/",
            //"system/STM32F0xx_HAL_Driver/Src/",
        ]
    }

    files:
    [
        "system/cmsis/inc/*.h",
        "system/STM32F0xx_HAL_Driver/inc/*.h",
        "system/STM32F0xx_HAL_Driver/Src/*.c",
        "system/startup_stm32f051x8.s",
        "system/system_stm32f0xx.c/",
    ]
}


