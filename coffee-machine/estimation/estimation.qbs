import qbs

QtApplication {
    cpp.cxxLanguageVersion: "c++11"
    cpp.defines: ["QT_DEPRECATED_WARNINGS",]
    consoleApplication: true
    files:
    [
        "main.cpp",
        "neuronnetwork.cpp",
        "neuronnetwork.h",
        "training.cpp",
    ]

    Group
    {
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
