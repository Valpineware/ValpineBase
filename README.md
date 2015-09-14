# Overview
A set of utilities for enhanced C++ Qt5 application development. The main features include (many are WIP):

* Easy-to-use unit testing framework which outputs to a file and can automatically launch the [QtTestReviewGUI](https://github.com/DanWatkins/QtTestReviewGUI) tool if installed on the system.
* Templated Property class which allows for custom setters/getters to be specified as lamdas if needed. Lamdas can also be attached to the property's onValueChanged signal.
* Many convenience classes that generally rely on RAII.
* Application settings management system with custom Qt Quick controls that sync with the settings.

# Build Instructions

The project may build and run with different configurations, but only the following are guaranteed to work.

### Requirements

* **Build Machine**
  * C++ Toolchain: GCC 4.9.2+ | MSVC 14.0+ | Clang 3.6+ (todo)
  * Qt: 5.5+


* **Target Machine**
  * OS: Windows 7+ | Ubuntu 15+ | OSX 10.10+ | Android 2.3+ | iOS 6.0+


### Usage

ValpineBase uses QBS for the build system. This means projects using ValpineBase also need to use QBS. The following steps are needed to connect ValpineBase to any QBS Product.

1. Clone ValpineBase as a submodule into your project's directory somewhere (reccomended, but not required).
2. Set **cpp.cxxLanguageVersion** to "c++14" or greater.
3. Set **cpp.includePaths** to include "&lt;path_to_valpine_base&gt;/Src/".
4. Add **Depends { name: "ValpineBase" }**.
5. To tell Qt Creator where to find ValpineBase QML files, add this property declaration: **property var qmlImportPaths: [ "&lt;path_to_valpine_base&gt;/Src/" ]**. To use a relative path, prepend the **path** property which contains an absolute path to the current QBS file's directory. For example, **property var qmlImportPaths: [ path + "/../Ext/ValpineBase/Src/" ]**. The relative path on it's own does not seem to be correctly interpreted by itself. Note this step is optional as it only allows for correct syntax highlighting and code-completion.

Here is a complete example QBS file:

```
import qbs 1.0

Application {
	name: "QtTestReviewGUI"

	cpp.cxxLanguageVersion: "c++14"
	cpp.includePaths: [
		"../Src/",
		"../Ext/ValpineBase/Src"
	]

	property var qmlImportPaths: [
		path + "/../Ext/ValpineBase/Src/",
	]

	Group {
		name: "C++"
		prefix: "**/"
		files: ["*.cpp", "*.h"]
	}

	Group {
		name: "Resources"

		prefix: "**/"
		files: ["*.qml*", "*.qrc"]
	}

	Depends { name: "cpp" }
	Depends { name: "Qt.core" }
	Depends { name: "Qt.quick" }
	Depends { name: "Qt.widgets" }

	Depends { name: "ValpineBase" }
	Depends { name: "QtTestReviewGUI_Lib" }
}


```
