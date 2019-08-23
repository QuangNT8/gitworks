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
        "Inc/",
        "Drivers/CMSIS/Device/ST/STM32F1xx/Include/",
        "Drivers/CMSIS/Include/",
        "Drivers/STM32F1xx_HAL_Driver/Inc/",
    ]

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:
        [
            "Inc/",
            "Drivers/CMSIS/Device/ST/STM32F1xx/Include/",
            "Drivers/",
            "Drivers/CMSIS/",
            "Drivers/CMSIS/Include/",
            "Drivers/STM32F1xx_HAL_Driver/Inc/",
            path,
        ]
    }

    files:
    [
        "Inc/stm32f1xx_hal_conf.h",
        "Drivers/CMSIS/Device/ST/STM32F1xx/Include/*.h",
        "Drivers/CMSIS/Include/*.h",
        "Drivers/STM32F1xx_HAL_Driver/Inc/*.h",
        "Src/system_stm32f1xx.c",
        "Src/syscalls.c",
        "system/Src/*.c",
        "Drivers/STM32F1xx_HAL_Driver/Src/*.c",
        "startup/startup_stm32f103xb.s",
        "../modules/stm32f1/core/*",
    ]
}


