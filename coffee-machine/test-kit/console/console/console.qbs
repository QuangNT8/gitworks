import qbs
StaticLibrary
{
    name: "console"
    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.quickcontrols2" }
    Depends { name: "Qt.charts" }
    Depends { name: "commons" }
    Depends { name: "core" }
    Depends { name: "home-icons" }
    cpp.cxxLanguageVersion: "c++11"

    cpp.includePaths:
    [
        project.sourceDirectory,
        product.sourceDirectory,
        qbs.targetOS.contains("windows") ? project.sourceDirectory+protoIncludePath : protoIncludePath
    ]
    cpp.libraryPaths: qbs.targetOS.contains("windows") ? [project.sourceDirectory+protoLibPath,] : [protoLibPath,]
    cpp.dynamicLibraries: ["protobuf"]
    Export
    {
        Depends { name: "cpp" }
        cpp.includePaths:
        [product.sourceDirectory]
    }
    files:
    [
        "*.cpp",
        "*.h",
        "operation/*.h",
        "operation/*.cpp",
    ]
}
