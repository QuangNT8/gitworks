import qbs 1.0
import qbs.FileInfo

Module
{
    Depends{name: "cpp"}
    property string optimization: "fast"

    cpp.defines: ["STM32F10X_LD_VL", "MCU_STM32F1"]
    cpp.positionIndependentCode: false
    cpp.enableExceptions: false
    cpp.executableSuffix: ".elf"
    cpp.cxxFlags: ["-std=c++11"]
    cpp.cFlags: ["-std=gnu99"]

    Properties
    {
        condition: qbs.buildVariant === "debug"
        cpp.defines: outer.concat(["DEBUG=1"])
        cpp.debugInformation: true
        cpp.optimization: "none"
    }

    Properties
    {
        condition: qbs.buildVariant === "release"
        cpp.debugInformation: false
        cpp.optimization: optimization
    }

    cpp.driverFlags:
        [
        "-mthumb",
        "-mcpu=cortex-m3",
        "-mfloat-abi=soft",
        "-fno-strict-aliasing",
        "-g3",
        "-Wall",
        "-mfpu=vfp",
        "-flto"
    ]

    cpp.commonCompilerFlags:
        [
        "-fdata-sections",
        "-ffunction-sections",
        "-fno-inline",
        "-flto"
    ]

    cpp.linkerFlags:
        [
        //"--specs=nano.specs",
        "--start-group",
        "--gc-sections",
        "-T" + path + "/system/linker/stm32f10x_flash.ld",
        "-lnosys",
        "-lgcc",
        "-lc",
        "-lstdc++",
        "-lm"
    ]

    Rule
    {
        inputs: ["application"]

        Artifact
        {
            filePath: project.buildDirectory + product.name + ".hex"
            fileTags: "hex"
        }

        prepare:
        {
            var GCCPath = ""
            var argsSize = [input.filePath]
            var argsObjcopy = ["-O", "ihex", input.filePath, output.filePath]

            var cmdSize = new Command("arm-none-eabi-size", argsSize)
            var cmdObjcopy = new Command("arm-none-eabi-objcopy", argsObjcopy)

            cmdSize.description = "Size of sections:"
            cmdSize.highlight = "linker"

            cmdObjcopy.description = "convert to bin..."
            cmdObjcopy.highlight = "linker"

            return [cmdSize, cmdObjcopy]
        }
    }

    Rule
    {
        inputs: ["hex"]

        Artifact
        {
            fileTags: "flash"
            filePath: "_"
        }

        prepare:
        {
            var argsSize = [input.filePath]
            var argsObjcopy = ["-O", "ihex", input.filePath, output.filePath]

            var cmdSize = new Command(project.gccPath+"arm-none-eabi-size", argsSize)
            var cmdObjcopy = new Command(project.gccPath+"arm-none-eabi-objcopy", argsObjcopy)

            cmdSize.description = "Size of sections:"
            cmdSize.highlight = "linker"

            cmdObjcopy.description = "convert to bin..."
            cmdObjcopy.highlight = "linker"

            return [cmdSize, cmdObjcopy]
        }
    }

}

