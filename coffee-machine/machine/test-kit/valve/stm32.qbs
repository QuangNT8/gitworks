import qbs

StaticLibrary
{
    name: "valve"
    Depends{name: "cpp"}
    Depends{name: "stm32f0"}
    Depends{name: "mdk"}
    Depends{name: "uart"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "controller.cpp",
        "controller.h",
    ]
}

