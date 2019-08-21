import qbs

StaticLibrary
{
	property string protoPath: project.sourceDirectory + "/core"
    Depends { name: "commons" }
    Depends { name: "proto" }

	consoleApplication: true
	cpp.cxxLanguageVersion: "c++11"
    cpp.includePaths:
    [
        product.sourceDirectory,
        project.sourceDirectory,
        protoPath,
        project.sourceDirectory + protoIncludePath,
    ]

    Export
    {
        Depends { name: "cpp" }
        cpp.includePaths:
        [
            project.sourceDirectory,
            product.sourceDirectory
        ]

        cpp.staticLibraries:
        [
            qbs.targetOS.contains("macos") ? "boost_system-mt" : "boost_system",
            "pthread",
            qbs.targetOS.contains("macos") ? "boost_thread-mt" : "boost_thread",
            "protobuf",
            "glog",
        ]

    }
	files:
	[
        "*.h",
        "*.cpp",
        "tcp/*.cpp","tcp/*.h",
        "serial/*.h","serial/*.cpp",
    ]
    Group
    {
        fileTagsFilter: "application"
        qbs.install: true
    }
}
