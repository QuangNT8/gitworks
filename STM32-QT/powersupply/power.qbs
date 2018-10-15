import qbs

Product
{
    property string boardId: "682249636"
    name: "power"
    //type: ["application", "hex", "flash"]
    type: ["application", "hex",]

    Depends { name:"cpp"}
    Depends {name: "stm32f0"}
    cpp.defines:
    [
       'STM32F051x8',
       '___int16_t_defined',
        '___int8_t_defined',
    ]

    cpp.includePaths:
        [
            'Inc/',
            'Drivers/STM32F0xx_HAL_Driver/Inc/',
            'Drivers/CMSIS/Device/ST/STM32F0xx/Include/',
            'Drivers/CMSIS/Include/',
        ]

    files:
        [
            'Inc/*.h/',
            'Drivers/CMSIS/Device/ST/STM32F0xx/Include/*.h',
            'Drivers/STM32F0xx_HAL_Driver/Inc/*.h',
            'Drivers/STM32F0xx_HAL_Driver/Src/*.c',
            'Src/*.c/',
            'main.c',
            "STM32F051R8Tx_FLASH.ld",
        ]
}


