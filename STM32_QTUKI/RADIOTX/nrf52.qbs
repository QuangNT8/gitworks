import qbs

Product
{
    name: "radiotx"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]

    property string boardId: '682249636'
    //property string boardId: '682284098'

    Depends{name: "cpp"}
    Depends{name: "nrf52"}
    Depends{name: "mdk"}
    Depends{name: "timer"}
    Depends{name: "uart"}
    Depends{name: "sadc"}
    Depends{name: "radio"}
    Depends{name: "AcMesurement"}

    cpp.includePaths: [project.sourceDirectory]

    files:["main.cpp","controller.h", "nrf52.cpp", "command.cpp","command.h"]
}

