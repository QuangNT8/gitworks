import qbs
CppApplication
{
    name: "console-ui"
    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.quickcontrols2" }
    Depends { name: "Qt.network" }
    Depends { name: "Qt.charts" }
    Depends { name: "core" }
    Depends { name: "console" }
    Depends { name: "commons" }
    Depends { name: "home-icons" }

    property string protoPath: project.sourceDirectory + "/core"
    cpp.defines:["QT_DEPRECATED_WARNINGS"]
    cpp.cxxLanguageVersion: "c++11"
    cpp.includePaths:
    [
        project.sourceDirectory,
        product.sourceDirectory,
        qbs.targetOS.contains("windows") ? project.sourceDirectory+protoIncludePath : protoIncludePath
    ]

    cpp.libraryPaths: qbs.targetOS.contains("windows") ? [project.sourceDirectory+protoLibPath,] : [protoLibPath,]
    cpp.staticLibraries: ["protobuf"]

    files:
    [
        "main.cpp",
    ]

    Group
    {
        Qt.core.resourcePrefix: "/";
        name: "main";
        fileTags: "qt.core.resource_data";
        files:["qml/*.qml"]
    }

    Group
    {
        Qt.core.resourcePrefix: "/commons";
        name: "commons";
        fileTags: "qt.core.resource_data";
        files:["commons/*.qml"]
    }

    Group
    {
        Qt.core.resourcePrefix: "/home";
        name: "home";
        fileTags: "qt.core.resource_data";
        files:["home/*.qml"]
    }

}
