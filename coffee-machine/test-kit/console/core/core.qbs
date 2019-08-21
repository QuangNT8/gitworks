import qbs

StaticLibrary
{
    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.quickcontrols2" }
    Depends { name: "commons" }
    Depends { name: "proto" }

    cpp.cxxLanguageVersion: "c++11"

	Export
	{
		Depends { name: "cpp" }
		cpp.includePaths:
		[
            product.sourceDirectory,
            project.sourceDirectory,
            qbs.targetOS.contains("windows") ? project.sourceDirectory+protoIncludePath : protoIncludePath
		]

        cpp.libraryPaths:
        [
            qbs.targetOS.contains("windows") ? project.sourceDirectory+protoLibPath : protoLibPath
        ]
	}

    cpp.includePaths:
    [
        product.sourceDirectory,
        project.sourceDirectory,
        qbs.targetOS.contains("windows") ? project.sourceDirectory+protoIncludePath : protoIncludePath
    ]

    files:
    [
        "*.cpp",
        "*.h",
        "tcp/*.h",
        "tcp/*.cpp",
    ]
}
