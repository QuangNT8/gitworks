import qbs

StaticLibrary
{
	Depends { name: "proto" }
    Depends
    {
        condition: project.useQt
        name: "Qt.core"
    }
    consoleApplication: true
	cpp.cxxLanguageVersion: "c++11"
    cpp.includePaths:
    [
        product.sourceDirectory,
        project.sourceDirectory + protoIncludePath
    ]

    Export
    {
        Depends { name: "cpp" }
        cpp.includePaths:
        [
            product.sourceDirectory,
            project.sourceDirectory + protoIncludePath
        ]
    }

    cpp.staticLibraries: ["protobuf"]

    files:
    [
        "comm.machine.proto",
        "comm.kit.proto",
        "config.h",
        "comm.proto",
        "data.bj.proto",
        "data.kit.proto",
    ]
}
