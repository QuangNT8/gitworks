import qbs 1.0

StaticLibrary
{
    name: "mdk"
    Depends { name:"cpp"}
    Depends { name:"nrf52"}

    cpp.includePaths:["system", "system/include"]

    Export
    {
        Depends { name:"cpp"}
        cpp.includePaths:["system", "system/include", path]
    }

    files:
    [
        "system/gcc_startup_nrf52.S",
        "system/system_nrf52.c",
        "hal/*",
    ]
}


