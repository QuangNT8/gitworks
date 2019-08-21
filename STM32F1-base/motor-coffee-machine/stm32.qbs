import qbs

Product
{
    name: "motor-coffee-machine"
//    type: ["application", "hex", "flash"]
    type: ["application", "hex"]
    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}
    Depends{name: "GPIO"}
    Depends{name: "RCservo"}
    cpp.includePaths: [project.sourceDirectory]
    cpp.defines:
    [
       '___int16_t_defined',
        '___int8_t_defined',
    ]
    files:[
        "define.h",
        "main.cpp",
        "motor.cpp",
        "motor.h",
        "motorADC.cpp",
        "motorGPIO.cpp",
        "motorPWM.cpp",
        "motorSPI.cpp",
        "command.cpp",
        "command.h",
        "pwm-step.mcud",
        "timer2.cpp",
    ]
}

