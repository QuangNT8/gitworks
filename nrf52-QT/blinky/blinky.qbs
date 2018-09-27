import qbs

Product
{
    property string boardId: "682249636"
    name: "blinky"
    type: ["application", "hex", "flash"]

    Depends { name:"cpp"}
    Depends { name:"nrf52-sdk"}
    Depends { name:"nrf52"}

    files:
    [
        "main.cpp",
    ]
}


