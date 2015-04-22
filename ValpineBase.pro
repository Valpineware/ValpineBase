TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += IDE/External \
        IDE/Library \
        IDE/Tests

IDE/Tests.depends = IDE/External
IDE/Tests.depends = IDE/Library
