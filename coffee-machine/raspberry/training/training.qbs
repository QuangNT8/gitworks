import qbs

CppApplication
{
    name: "ann-training"
    Depends { name: "core" }
    Depends { name: "commons" }
    Depends { name: "ann" }

    cpp.cxxLanguageVersion: "c++11"
    consoleApplication: true
    cpp.includePaths: [product.sourceDirectory]
    cpp.staticLibraries:
    [
        qbs.targetOS.contains("macos") ? "boost_system-mt" : "boost_system",
        "pthread",
        qbs.targetOS.contains("macos") ? "boost_thread-mt" : "boost_thread",
        "protobuf",
        "glog",
    ]

    files:
    [
        "*.h",
        "*.cpp",
    ]

    Group
    {
        name: "data"
        qbs.install: true
        files:
        [
            "trainingData.txt",
            "trainingData2.txt",
            "testingData1.txt",
            "testingData2.txt",
//            "trained_model.pb",
            "testingData4.txt",
            "testingData3.txt",
        ]
    }

    Group
    {
        fileTagsFilter: "application"
        qbs.install: true
    }
}
