import qbs
import qbs.FileInfo
import qbs.TextFile

DynamicLibrary
{
    name: "home-icons"
    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
    Depends { name: "Qt.quickcontrols2" }
    Depends { name: "commons" }
    Depends { name: "core" }

    Qt.core.resourceFileBaseName: "font"
    Qt.core.resourcePrefix: "/font";

	files:
	[
        "README.txt",
        "flaticon.css",
        "flaticon.h",
    ]

	Group
	{
		name: "font";
		fileTags: "qt.core.resource_data";
		files:
		[
			"Flaticon.eot",
			"Flaticon.svg",
			"Flaticon.ttf",
			"Flaticon.woff"
		]
	}

    FileTagger
    {
        patterns: ["flaticon.css"]
        fileTags: ["flaticon"]
    }

    Rule
    {
        id: flatfont
        inputs: "flaticon"

        Artifact
        {
            fileTags: ["cpp"]
            filePath: FileInfo.path(input.filePath) +"/flaticon.cpp"
        }

        prepare:
        {
            var cmd = new JavaScriptCommand();
            cmd.description = "generating flaticon: " + input.filePath;
            cmd.highlight = "codegen";
            cmd.workingDirectory = FileInfo.path(input.filePath)

            cmd.sourceCode = function()
            {
                var inFile = new TextFile(input.filePath);
                var outFile = new TextFile(FileInfo.path(input.filePath) +"/flaticon.cpp", TextFile.WriteOnly);
                var re1 = new RegExp('^\\.flaticon-([^:]*):before\\s\\{\\scontent:\\s\"\\\\(.*)\";\\s\\}$');

                outFile.writeLine('#include "flaticon.h"')
                outFile.writeLine('void icon::Flaticon::setData_()');
                outFile.writeLine('{');
                while (!inFile.atEof())
                {
                    var line = inFile.readLine();
                    var match = re1.exec(line);
                    if (match!=null)
                    {
                        outFile.writeLine('\ticons_["' + match[1]+ '"] = QChar(0x' + match[2] + ');');
                    }
                }
                outFile.writeLine('}')
                inFile.close();
                outFile.close();
            }
            return [cmd];
        }
    }
}
