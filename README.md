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

ValpineBase uses QBS for the build system. This means projects using ValpineBase also need to use QBS. There are 4 simple steps to connect ValpineBase to any QBS Product QML type.

1. Clone ValpineBase as a submodule into your project's directory somewhere (if using git).
2. Set **cpp.cxxLanguageVersion** to "c++14" or greater on the QML Product.
3. Set **cpp.includePaths** to include ""&lt;path_to_valpine_base&gt;/Src/" on the QML Product.
4. Add **Depends { name: "ValpineBase" }** somewhere in the QML Product.
5. Include ValpineBase header files with **#include &lt;ValpineBase/Property.h&gt;** for example.
