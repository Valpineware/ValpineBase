//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Class_h
#define vbase_test_Class_h

#include <QObject>
#include <QtCore/QElapsedTimer>
#include <QtCore/QString>

namespace vbase { namespace test {

class Suite;

class Class : public QObject
{
	Q_OBJECT

public:
	Suite* hostSuite;
	QElapsedTimer executionTimer;
	QString currentlyExecutingMethodName;
};
}}

#endif
