import qbs 1.0

StaticLibrary {
    name: "ValpineBase"
	
	cpp.cxxLanguageVersion: "c++14"
	cpp.includePaths: [
		"../../Ext/",
        "../../Ext/gmock/gtest/include",
        "./"
	]

    Group {
        name: "C++"
        prefix: "**/"
        files: ["*.h", "*.cpp"]
    }

    Group {
        name: "Resources"
        prefix: "**/"
        files: ["*.qml", "*.js", "*.qrc"]
    }

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
    Depends { name: "Qt.quick" }
	Depends { name: "Qt.network" }

	property string version: "0.3.0"

	cpp.defines: [
		"ValpineBase_Version=" + version
	]
}
