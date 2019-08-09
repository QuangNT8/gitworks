import qbs

Product
{
    name: "motor-controller"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]

    property string boardId: '682249636'
    Depends{name: "cpp"}
    Depends{name: "nrf52"}
    Depends{name: "mdk"}
    Depends{name: "uart"}
    Depends{name: "timer"}

    cpp.includePaths: [project.sourceDirectory]
    files:["controller.h", "controller.cpp", "nrf52.cpp", "main.cpp", "command.cpp", "command.h"]
}

