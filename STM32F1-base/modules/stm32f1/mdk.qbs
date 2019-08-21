import qbs 1.0

StaticLibrary
{
    name: "mdk"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}
    cpp.defines:
    [
       'STM32F103xB',
       '___int16_t_defined',
       '___int8_t_defined',
    ]

    cpp.includePaths:
    [
        "system/Inc",
        "system/cmsis/Device/ST/STM32F1xx/Include/",
        "system/cmsis/Include",
        "system/STM32F1xx_HAL_Driver/Inc/",
    ]

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:
        [
            "system/",
            "system/Inc",
            "system/cmsis/Inc",
            "system/cmsis/Include",
            "system/STM32F1xx_HAL_Driver/Inc/",
            "system/cmsis/Device/ST/STM32F1xx/Include/",
            path,
        ]
    }

    files:
    [
        "system/cmsis/Include/*.h",
        "system/STM32F1xx_HAL_Driver/inc/*.h",
        "system/STM32F1xx_HAL_Driver/Src/*.c",
        "system/startup/startup_stm32f103xb.s",
        "system/Src/system_stm32f1xx.c",
        "system/Src/*.c",
        "system/Inc/*.h",
        "system/cmsis/Device/ST/STM32F1xx/Include/*.h",
        "core/*",
    ]
}


