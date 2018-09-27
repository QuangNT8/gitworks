import qbs

Product
{
    property string boardId: "682249636"
    name: "RadioTX"
    type: ["application", "hex", "flash"]

    Depends { name:"cpp"}
    Depends { name:"nrf52"}

    cpp.defines:
    [
        'BOARD_PCA10040',
        'BSP_UART_SUPPORT',
        'CONFIG_GPIO_AS_PINRESET',
        'FLOAT_ABI_HARD',
        'NRF52',
        'NRF52832_XXAA',
        "NRF52_PAN_74",
        'SWI_DISABLE0'
    ]

    cpp.linkerFlags:
        [
        "-Wl,-Map="+buildDirectory+"/"+name+".map",
        "-mthumb", "-mabi=aapcs",
        "-L" + project.sdkPath + "components/toolchain/gcc",
        "-T"+ path +"/transmitter_gcc_nrf52.ld",
        "-mcpu=cortex-m4",
        "-mfloat-abi=hard", "-mfpu=fpv4-sp-d16",
        "-Wl,--gc-sections",
        "--specs=nano.specs","-lc","-lnosys","-lm"
    ]

    cpp.includePaths:
        [
        project.sdkPath + 'components/drivers_nrf/common',
        path,
        project.sdkPath + 'components/libraries/experimental_section_vars',
        project.sdkPath + 'components/drivers_nrf/radio_config',
        project.sdkPath + 'components/drivers_nrf/uart',
        project.sdkPath + 'components/libraries/experimental_memobj',
        project.sdkPath + 'components/libraries/atomic',
        project.sdkPath + 'components/libraries/fifo',
        project.sdkPath + 'components/device',
        project.sdkPath + 'components/libraries/uart',
        project.sdkPath + 'components/toolchain/cmsis/include',
        project.sdkPath + 'components/libraries/balloc',
        project.sdkPath + 'components/drivers_nrf/hal',
        project.sdkPath + 'components',
        project.sdkPath + 'components/libraries/scheduler',
        project.sdkPath + 'components/libraries/strerror',
        project.sdkPath + 'components/libraries/experimental_log/src',
        project.sdkPath + 'external/fprintf',
        project.sdkPath + 'components/drivers_nrf/nrf_soc_nosd',
        project.sdkPath + 'external/segger_rtt',
        project.sdkPath + 'components/toolchain/gcc',
        project.sdkPath + 'components/drivers_nrf/delay',
        project.sdkPath + 'components/drivers_nrf/gpiote',
        project.sdkPath + 'components/libraries/bsp',
        project.sdkPath + 'components/boards',
        project.sdkPath + 'components/libraries/timer',
        project.sdkPath + 'components/libraries/button',
        project.sdkPath + 'components/libraries/util',
        project.sdkPath + 'components/libraries/experimental_log',
        project.sdkPath + 'components/toolchain',
        'config',
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
        project.sdkPath + 'components/libraries/button/app_button.c',
        project.sdkPath + 'components/libraries/util/app_error.c',
        project.sdkPath + 'components/libraries/util/app_error_weak.c',
        project.sdkPath + 'components/libraries/fifo/app_fifo.c',
        project.sdkPath + 'components/libraries/scheduler/app_scheduler.c',
        project.sdkPath + 'components/libraries/timer/app_timer.c',
        project.sdkPath + 'components/libraries/uart/app_uart_fifo.c',
        project.sdkPath + 'components/libraries/util/app_util_platform.c',
        project.sdkPath + 'components/libraries/util/nrf_assert.c',
        project.sdkPath + 'components/libraries/balloc/nrf_balloc.c',
        project.sdkPath + 'external/fprintf/nrf_fprintf.c',
        project.sdkPath + 'external/fprintf/nrf_fprintf_format.c',
        project.sdkPath + 'components/libraries/experimental_memobj/nrf_memobj.c',
        project.sdkPath + 'components/libraries/strerror/nrf_strerror.c',
        project.sdkPath + 'components/libraries/uart/retarget.c',
        project.sdkPath + 'components/drivers_nrf/common/nrf_drv_common.c',
        project.sdkPath + 'components/drivers_nrf/gpiote/nrf_drv_gpiote.c',
        project.sdkPath + 'components/drivers_nrf/uart/nrf_drv_uart.c',
        project.sdkPath + 'components/drivers_nrf/radio_config/radio_config.c',
        project.sdkPath + 'components/libraries/bsp/bsp.c',
        project.sdkPath + 'components/libraries/bsp/bsp_nfc.c',
        project.sdkPath + 'external/segger_rtt/SEGGER_RTT.c',
        project.sdkPath + 'external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c',
        project.sdkPath + 'external/segger_rtt/SEGGER_RTT_printf.c',
        project.sdkPath + 'components/toolchain/gcc/gcc_startup_nrf52.S',
        project.sdkPath + 'components/toolchain/system_nrf52.c',

        'main.c',
        'config/sdk_config.h',
        "transmitter_gcc_nrf52.ld",
    ]
}


