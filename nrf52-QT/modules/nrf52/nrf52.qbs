import qbs 1.0
import qbs.FileInfo

Module
{
    Depends { name:"cpp"}

    cpp.warningLevel: "default" //"all" // or "none", "default"
    cpp.treatWarningsAsErrors: false
    cpp.executableSuffix: ".out"
    cpp.positionIndependentCode: false
    cpp.enableRtti: false

    Properties
    {
        condition: qbs.buildVariant === "debug"
        cpp.commonCompilerFlags:
        [
            "-mcpu=cortex-m4", "-mthumb", "-mabi=aapcs", "-mfloat-abi=hard" , //'-mfpu=fpv4-sp-d16',
            "-fno-builtin",
            "-fdata-sections","-ffunction-sections","-fno-strict-aliasing",
            "-Wno-inconsistent-missing-destructor-override",
        ]
        cpp.debugInformation: true
        cpp.optimization: "none" // "none" or "fast"
    }

    Properties
    {
        condition: qbs.buildVariant === "release"
        cpp.commonCompilerFlags:
            [
            "-mcpu=cortex-m4", "-mthumb", "-mabi=aapcs", "-mfloat-abi=hard", //'-mfpu=fpv4-sp-d16',
            "-fno-builtin",
            "-fdata-sections","-ffunction-sections","-fno-strict-aliasing",
            "-Wno-inconsistent-missing-destructor-override",
        ]
        cpp.debugInformation: false
        cpp.optimization: "small"
    }

    cpp.linkerMode: "manual"
    cpp.linkerPath: project.linkerPath

    Rule
    {
        id: hex
        inputs: "application"
        Artifact
        {
            fileTags: ["hex"]
            filePath: FileInfo.baseName(input.filePath) + ".hex"
        }
        prepare:
        {
            var argsHex = ["-O", "ihex", input.filePath, output.filePath];
            var cmdHex = new Command(project.gccPath+"arm-none-eabi-objcopy", argsHex);
            cmdHex.description = "converting to hex: "+FileInfo.fileName(output.filePath);
            cmdHex.highlight = "linker";

            var argsBin = ["-O", "binary", input.filePath, output.filePath];
            var cmdBin = new Command(project.gccPath+"arm-none-eabi-objcopy", argsBin);
            cmdBin.description = "converting to bin: "+FileInfo.fileName(output.filePath);
            cmdBin.highlight = "linker";

            var argsSize = [input.filePath];
            var cmdSize = new Command(project.gccPath+"arm-none-eabi-size", argsSize);
            cmdSize.highlight = "linker";
            cmdSize.silent = true;

            return [cmdBin, cmdSize, cmdHex];
        }
    }

    Rule
    {
        id: flash
        inputs: "hex"
        Artifact
        {
            fileTags: ["flash"]
            filePath: "_"
        }
        prepare:
        {
            var args2 = ["-s", product.boardId, "--program", input.filePath , "-f","nrf52", "--sectorerase"];
            var cmd2 = new Command(project.nrfjprogPath, args2);

            var args3 = ["-s", product.boardId, "--reset"];
            var cmd3 = new Command(project.nrfjprogPath, args3);

            cmd2.description = "flashing : " + FileInfo.fileName(input.filePath);
            cmd2.highlight = "linker";
            return [cmd2, cmd3];
        }
    }

}

