import qbs 1.0

Application {
    name: "ValpineBaseTests"
    consoleApplication: true

	cpp.cxxLanguageVersion: "c++14"
	cpp.includePaths: [
		"../../Ext/",
        "../../Ext/gmock/gtest/include",
        "../Src/"
	]
	
    Group {
        name: "C++"
        prefix: "**/"
        files: ["*.cpp", "*.h"]
    }

    Depends { name: "cpp" }
    Depends { name: "Qt.core" }
	Depends { name: "Qt.testlib" }

    Depends { name: "ValpineBase" }
}
