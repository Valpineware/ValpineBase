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
	//TODO get a better name for this. feedback? idk
	struct Result
    {
    public:
        virtual ~Result() {}
		int executionTime = 0;   //in ms
    };


	struct ResultFailure : public Result
    {
    public:
		QStringList message;
		QString filepath;
		int lineNumber = -1;
    };


    /**
     * Indicates a test failure due to an assert being triggered.
     */
	class TestAssertException/* : public std::exception*/
    {
    public:
		TestAssertException() {}

		/*virtual*/ const char* what() const noexcept
        {
            static const std::string message = "Test failure occured. See related Suite instance for details.";

            return message.c_str();
        }

        //TODO revert back to unique_ptr as well eventually
        ResultFailure* pResultFailure;
    };
}}

#endif
