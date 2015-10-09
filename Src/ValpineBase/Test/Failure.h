//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Message_h
#define vbase_test_Message_h

#include <exception>
#include <memory>
#include <QString>
#include <QStringList>
#include <QMetaMethod>

namespace vbase { namespace test {

struct Failure
{
public:
	QStringList details;
	QString filepath;
	int lineNumber = -1;

	enum class Type
	{
		Warn = 0,
		Error = 1
	} type = Type::Error;

	QString typeName() const
	{
		switch (type)
		{
		case Type::Warn:
			return "warning";
		case Type::Error:
			return "error";
		}

		return "error";
	}
};


/**
 * Indicates a test failure due to an assert being triggered.
 */
class TestAssertException {};

}}

#endif
