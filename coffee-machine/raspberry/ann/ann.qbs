import qbs

StaticLibrary
{
	property string protoPath: project.sourceDirectory + "/core"
    Depends { name: "commons" }

	consoleApplication: true
	cpp.cxxLanguageVersion: "c++11"
    cpp.includePaths:
    [
        product.sourceDirectory,
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
    }
	files:
	[
        "*.h",
        "*.cpp",
    ]
}
