import qbs 1.0
import qbs.FileInfo

Module
{
    Depends { name:"cpp"}
    cpp.defines:
    [
        "MCU_NRF52",
        'NRF52_PAN_12',
        'NRF52_PAN_15',
        'NRF52_PAN_58',
        'SWI_DISABLE0',
        'NRF52_PAN_20',
        'NRF52_PAN_54',
        'NRF52',
        'NRF52_PAN_51',
        'NRF52_PAN_36',
        'CONFIG_GPIO_AS_PINRESET',
        'NRF52_PAN_64',
        'NRF52_PAN_55',
        'BOARD_PCA10040',
        'NRF52_PAN_31',
        'NRF52832',
        'BSP_DEFINES_ONLY'
    ]

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
            "-Wall"
        ]
        cpp.debugInformation: true
        cpp.optimization: "none" // "none" or "fast"
    }

    Properties
    {
        condition: qbs.buildVariant === "release"
        cpp.commonCompilerFlags:
            [
            "-mcpu=cortex-m4", "-mthumb", "-mabi=aapcs", "-mfloat-abi=hard", '-mfpu=fpv4-sp-d16',
            "-fno-builtin",
            "-fdata-sections","-ffunction-sections","-fno-strict-aliasing",
            "-Wall"
        ]
        cpp.debugInformation: false
        cpp.optimization: "small"
    }

    cpp.linkerFlags:
        [
        "-Wl,-Map="+buildDirectory+"/"+name+".map",
        "-mthumb", "-mabi=aapcs",
        "-L"+path+"/system",
        "-T"+path+"/system/nrf52_xxaa.ld",
        "-mcpu=cortex-m4",
        "-mfloat-abi=hard", "-mfpu=fpv4-sp-d16",
        "-Wl,--gc-sections",
        "--specs=nano.specs","-lc","-lnosys","-lm"
    ]
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
            var argsProg = ["-s", product.boardId, "--program", input.filePath , "-f","nrf52", "--sectorerase"];
            //var argsProg = ["-s", "682510516", "--program", input.filePath , "-f","nrf52", "--sectorerase"];
            var cmdProg = new Command(project.nrfjprogPath, argsProg);
            cmdProg.description = "flashing : " + FileInfo.fileName(input.filePath);
            cmdProg.highlight = "linker";

            var argsReset = ["-s", product.boardId, "--reset"];
            var cmdReset = new Command(project.nrfjprogPath, argsReset);
            return [cmdProg, cmdReset];
        }
    }
}

