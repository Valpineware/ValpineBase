TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += Src/External \
		Src/ValpineBase \
		Src/Tests

IDE/Tests.depends = IDE/External
IDE/Tests.depends = IDE/Library
