import qbs

Project
{
    minimumQbsVersion: "1.7.1"

    CppApplication
    {
        name: "console"
        Depends { name: "Qt.core" }
        Depends { name: "Qt.quick" }
        Depends { name: "Qt.charts" }
        Depends { name: "Qt.quickcontrols2" }
        Depends { name: "Qt.serialport" }

        property pathList qmlImportPaths: []

        cpp.includePaths:
        [
            product.sourceDirectory
        ]

        cpp.cxxLanguageVersion: "c++11"

        cpp.defines: ["QT_DEPRECATED_WARNINGS",]

        files:
        [
            "*.cpp",
            "*.h",
            "motor-controller.cfg",
            "qml/qml.qrc",
            "qml/*.qml",
            "core/*.cpp",
            "core/*.h",
            "serial/*.cpp",
            "serial/*.h",
        ]

        Group {     // Properties for the produced executable
            fileTagsFilter: product.type
            qbs.install: true
        }
    }
}
