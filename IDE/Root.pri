MY_COMPILER = UnknownCompiler

gcc {
	warning(Doing gcc)
	MY_COMPILER = gcc
}
else:msvc {
	warning(Doing msvc)
	MY_COMPILER = msvc
}
