//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Class_h
#define vbase_test_Class_h

#include <QtCore/QObject>
#include <QtCore/QElapsedTimer>
#include <QtCore/QString>

#include <ValpineBase/ValpineBase.h>

namespace vbase { namespace test {

class Suite;

class Class : public QObject
{
	Q_OBJECT

public:
	Suite* _hostSuite;
	QString _currentlyExecutingMethodName;
};

END_NAMESPACE
END_NAMESPACE

#endif
