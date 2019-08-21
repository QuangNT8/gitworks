import qbs

CppApplication
{
    name: "serial-test"
    Depends { name: "core" }
    Depends { name: "commons" }

    cpp.cxxLanguageVersion: "c++11"
    consoleApplication: true
    cpp.includePaths: [product.sourceDirectory]
    cpp.staticLibraries: ["glog" ]
    files:
    [
        "*.h",
        "*.cpp",
    ]

    Group
    {
        fileTagsFilter: "application"
        qbs.install: true
    }
}
