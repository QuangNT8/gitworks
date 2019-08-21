import qbs

StaticLibrary
{
    name: "motor"
    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "adc.cpp",
        "calibrate.cpp",
        "controller.cpp",
        "controller.h",
        "current.cpp",
        "define.h",
        "generator.cpp",
        "generator.h",
        "command.cpp",
        "command.h",
        "gpio.cpp",
        "position.cpp",
        "pwm.cpp",
        "spi.cpp",
        "machine-v0.3.mcud",
    ]
}

