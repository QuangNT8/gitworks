import qbs

Project
{
    name: "bj-machine"
    minimumQbsVersion: "1.7.1"

    property string protocPath: "/usr/local/bin/protoc"
    property string protoIncludePath: "/usr/local/include"
    property string protoLibPath: "/usr/local/lib"
    property bool useQt: false

    references:
    [
        "../commons/commons.qbs",
        "core/core.qbs",
        "ann/ann.qbs",
        "machine/machine.qbs",
//        "training/training.qbs",
    ]
    qbsSearchPaths: ["../commons/"]
}
