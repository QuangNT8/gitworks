import qbs 1.0

StaticLibrary
{
    name: "stm32f1-core"
    Depends { name:"cpp"}
    Depends { name:"stm32f1"}

    cpp.includePaths:
    [
        "system/cmsis",
        "system/cmsis_boot",
        "system/cmsis_boot/statup"
    ]

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:
        [
            "system/cmsis",
            "system/cmsis_boot",
            "system/cmsis_boot/statup"
        ]
    }

    files:["system/**"]
}


