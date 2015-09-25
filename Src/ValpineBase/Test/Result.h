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
	struct Failure
    {
    public:
		QStringList message;
		QString filepath;
		int lineNumber = -1;
    };


    /**
     * Indicates a test failure due to an assert being triggered.
     */
	class TestAssertException {};
}}

#endif
