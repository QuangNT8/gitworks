import qbs

CppApplication
{
    name: "machine"
    Depends { name: "core" }
    Depends { name: "commons" }
    Depends { name: "ann" }

    cpp.cxxLanguageVersion: "c++11"
    consoleApplication: true
    cpp.includePaths: [product.sourceDirectory]
    cpp.staticLibraries: ["glog" ]
    files:
    [
        "*.h",
        "*.cpp",
        "operation/*.h",
        "operation/*.cpp",
        "utils/*.h",
        "utils/*.cpp",
    ]

    Group
    {
        name: "data"
        qbs.install: true
        files:
        [
            "projects.pb",
            "model_default.pb",
        ]
    }

    Group
    {
        fileTagsFilter: "application"
        qbs.install: true
    }
}
