//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_AssertException_h
#define vbase_test_AssertException_h

#include <exception>
#include <memory>
#include <QString>
#include <QStringList>
#include <QMetaMethod>

namespace vbase { namespace test
{
	struct Message
    {
    public:
		QStringList details;
		QString filepath;
		int lineNumber = -1;

		enum class Type
		{
			Debug = 0,
			Warn = 1,
			Error = 2
		} type = Type::Error;
    };


    /**
     * Indicates a test failure due to an assert being triggered.
     */
	class TestAssertException {};
}}

#endif
