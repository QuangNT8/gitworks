import qbs

Product
{
    name: "testosc"
    type: ["application", "hex", "flash"]
    //type: ["application", "hex"]

    property string boardId: '682415582'
    //property string boardId: '682249636'

    Depends{name: "cpp"}
    Depends{name: "nrf52"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:[
        "controller.h",
        "controller.cpp",
        "nrf52.cpp",
        "main.cpp",
        "command.cpp",
        "command.h",
        "oscilloscope.h",
		"define.h"
    ]
}
