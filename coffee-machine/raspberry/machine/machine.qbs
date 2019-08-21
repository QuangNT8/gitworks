import qbs

CppApplication
{
    name: "machine"
    Depends { name: "core" }
    Depends { name: "ann" }
    Depends { name: "commons" }

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
    ]

    Group
    {
        name: "data"
        qbs.install: true
        files:
        [
            "data.pb",
            "trained_model.pb"
        ]
    }

    Group
    {
        qbs.installDir: "files"
        qbs.install: true
        name: "files";
        files:[
            "files/*.*"
        ]
    }    
    Group
    {
        fileTagsFilter: "application"
        qbs.install: true
    }
}
