import qbs

StaticLibrary
{
    name: "kit"
    Depends{name: "cpp"}
    Depends{name: "stm32f1"}
    Depends{name: "mdk"}

    cpp.includePaths: [project.sourceDirectory]
    files:
    [
        "kit-controller.cpp",
        "kit-controller.h",
    ]
}

