import qbs
import qbs.FileInfo

Module
{
    Depends {name: "cpp"}
    cpp.includePaths:[path]

    FileTagger
    {
        patterns: "*.proto"
        fileTags: ["proto"]
    }

    Rule
    {
        id: proto
        inputs: "proto"

        Artifact
        {
            fileTags: ["cpp"]
			filePath: FileInfo.path(input.filePath) +"/"+ FileInfo.completeBaseName(input.filePath) + ".pb.cc"
        }

        Artifact
        {
            fileTags: ["hpp"]
			filePath: FileInfo.path(input.filePath) +"/"+ FileInfo.completeBaseName(input.filePath) + ".pb.h"
        }

        prepare:
        {
            var args =
            [
                "--cpp_out=" + FileInfo.path(input.filePath) ,
                "--proto_path=" + product.protoPath,
                "--proto_path=" + FileInfo.path(input.filePath),
                input.filePath
            ];

            var cmd = new Command(project.protocPath, args);
			//var cmd = new Command("protoc", args);

            cmd.description = "generating protocol: " + input.filePath;
            cmd.highlight = "codegen";
            cmd.workingDirectory = FileInfo.path(input.filePath)
            return cmd;
        }
    }
}
