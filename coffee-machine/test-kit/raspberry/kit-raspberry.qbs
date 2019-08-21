import qbs

Project
{
    name: "kit-machine"
    minimumQbsVersion: "1.7.1"

    property string protocPath: "/usr/local/bin/protoc"
    property string protoIncludePath: "/usr/local/include"
    property string protoLibPath: "/usr/local/lib"
    property bool useQt: false

    references:
    [
        "../../raspberry/ann/ann.qbs",
        "../../commons/commons.qbs",
        "core/core.qbs",
        "machine/machine.qbs",
    ]
    qbsSearchPaths: ["../../commons/"]
}
