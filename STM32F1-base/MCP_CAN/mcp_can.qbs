import qbs 1.0

StaticLibrary
{
    name: "mcp_can"
    Depends { name:"cpp"}
    Depends { name:"stm32f0"}
    Depends { name:"mdk"}
    Depends{name: "uart"}
    cpp.defines:
    [
       'STM32F051x8',
       '___int16_t_defined',
        '___int8_t_defined',
        'HAL_SPI_MODULE_ENABLED',
    ]
    files:
    [       
		"mcp_can.h",
		"mcp_can_dfs.h",
        "mcp_can.cpp"
    ]
}


