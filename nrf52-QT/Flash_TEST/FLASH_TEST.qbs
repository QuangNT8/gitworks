import qbs

Product
{
    property string boardId: "682249636"
    //property string boardId: "682284098"
    name: "FLASH_TEST"
//    type: ["application", "hex", "flash"]
     type: ["application", "hex",]
    Depends { name:"cpp"}
    Depends { name:"nrf52"}

    cpp.defines:
    [
        'BOARD_PCA10040',
        'BSP_DEFINES_ONLY',
        'BSP_UART_SUPPORT',
        'CONFIG_GPIO_AS_PINRESET',
        'FLOAT_ABI_HARD',
        'NRF52',
        'NRF52832_XXAA',
        "NRF52_PAN_74",
        'SWI_DISABLE0',
    ]

    cpp.linkerFlags:
        [
        "-Wl,-Map="+buildDirectory+"/"+name+".map",
        "-mthumb", "-mabi=aapcs",
        "-L" + project.sdkPath + "components/toolchain/gcc",
        "-T"+ path +"/flashwrite_gcc_nrf52.ld",
        "-mcpu=cortex-m4",
        "-mfloat-abi=hard", "-mfpu=fpv4-sp-d16",
        "-Wl,--gc-sections",
        "--specs=nano.specs","-lc","-lnosys","-lm",
    ]

    cpp.includePaths:
        [
            'config',
            path,
            project.sdkPath + 'components',
            project.sdkPath + 'components/libraries/cli',
            project.sdkPath + 'components/libraries/scheduler',
            project.sdkPath + 'components/libraries/experimental_log',
            project.sdkPath + 'components/libraries/queue',
            project.sdkPath + 'components/libraries/pwr_mgmt',
            project.sdkPath + 'components/libraries/strerror',
            project.sdkPath + 'components/drivers_nrf/delay',
            project.sdkPath + 'components/toolchain/cmsis/include',
            project.sdkPath + 'components/libraries/timer',
            project.sdkPath + 'components/libraries/util',
            project.sdkPath + 'components/drivers_nrf/uart',
            project.sdkPath + 'components/libraries/balloc',
            project.sdkPath + 'components/libraries/cli/uart',
            project.sdkPath + 'components/libraries/bsp',
            project.sdkPath + 'components/device',
            project.sdkPath + 'components/libraries/experimental_section_vars',
            project.sdkPath + 'components/libraries/mutex',
            project.sdkPath + 'components/libraries/experimental_log/src',
            project.sdkPath + 'external/segger_rtt',
            project.sdkPath + 'components/libraries/experimental_ringbuf',
            project.sdkPath + 'components/libraries/atomic',
            project.sdkPath + 'components/boards',
            project.sdkPath + 'components/drivers_nrf/hal',
            project.sdkPath + 'components/libraries/experimental_memobj',
            project.sdkPath + 'components/toolchain/gcc',
            project.sdkPath + 'components/toolchain',
            project.sdkPath + 'components/drivers_nrf/common',
            project.sdkPath + 'components/drivers_nrf/clock',
            project.sdkPath + 'components/drivers_nrf/nrf_soc_nosd',
            project.sdkPath + 'external/fprintf',
        ]

    files:
        [
            project.sdkPath + 'components/libraries/experimental_log/src/nrf_log_backend_rtt.c',
            project.sdkPath + 'components/libraries/experimental_log/src/nrf_log_backend_serial.c',
            project.sdkPath + 'components/libraries/experimental_log/src/nrf_log_backend_uart.c',
            project.sdkPath + 'components/libraries/experimental_log/src/nrf_log_default_backends.c',
            project.sdkPath + 'components/libraries/experimental_log/src/nrf_log_frontend.c',
            project.sdkPath + 'components/libraries/experimental_log/src/nrf_log_str_formatter.c',
            project.sdkPath + 'components/boards/boards.c',
            project.sdkPath + 'components/libraries/util/app_error.c',
            project.sdkPath + 'components/libraries/util/app_error_weak.c',
            project.sdkPath + 'components/libraries/scheduler/app_scheduler.c',
            project.sdkPath + 'components/libraries/timer/app_timer.c',
            project.sdkPath + 'components/libraries/util/app_util_platform.c',
            project.sdkPath + 'components/libraries/util/nrf_assert.c',
            project.sdkPath + 'components/libraries/balloc/nrf_balloc.c',
            project.sdkPath + 'components/libraries/cli/nrf_cli.c',
            project.sdkPath + 'components/libraries/cli/uart/nrf_cli_uart.c',
            project.sdkPath + 'external/fprintf/nrf_fprintf.c',
            project.sdkPath + 'external/fprintf/nrf_fprintf_format.c',
            project.sdkPath + 'components/libraries/experimental_memobj/nrf_memobj.c',
            project.sdkPath + 'components/libraries/pwr_mgmt/nrf_pwr_mgmt.c',
            project.sdkPath + 'components/libraries/queue/nrf_queue.c',
            project.sdkPath + 'components/libraries/experimental_ringbuf/nrf_ringbuf.c',
            project.sdkPath + 'components/libraries/experimental_section_vars/nrf_section_iter.c',
            project.sdkPath + 'components/libraries/strerror/nrf_strerror.c',
            project.sdkPath + 'components/drivers_nrf/clock/nrf_drv_clock.c',
            project.sdkPath + 'components/drivers_nrf/common/nrf_drv_common.c',
            project.sdkPath + 'components/drivers_nrf/uart/nrf_drv_uart.c',
            project.sdkPath + 'components/drivers_nrf/nrf_soc_nosd/nrf_nvic.c',
            project.sdkPath + 'components/drivers_nrf/hal/nrf_nvmc.c',
            project.sdkPath + 'components/drivers_nrf/nrf_soc_nosd/nrf_soc.c',
            project.sdkPath + 'external/segger_rtt/SEGGER_RTT.c',
            project.sdkPath + 'external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c',
            project.sdkPath + 'external/segger_rtt/SEGGER_RTT_printf.c',
            project.sdkPath + 'components/toolchain/gcc/gcc_startup_nrf52.S',
            project.sdkPath + 'components/toolchain/system_nrf52.c',
            'main.c',
            'config/sdk_config.h',
            "flashwrite_gcc_nrf52.ld",
        ]
}


