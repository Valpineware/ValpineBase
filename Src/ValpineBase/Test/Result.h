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

#include <ValpineBase/Property.h>

namespace vbase { namespace test
{
    class Result
    {
    public:
        virtual ~Result() {}
        Property<QMetaMethod> pTestMethod;
        Property<int> pExecutionTime = 0;   //in ms
    };


    class ResultFailure : public Result
    {
    public:
        Property<QStringList> pMessage;
        Property<QString> pFilepath;
        Property<int> pLineNumber = -1;
    };


    /**
     * Indicates a test failure due to an assert being triggered.
     */
    class TestFailureException/* : public std::exception*/
    {
    public:
        TestFailureException() {}

		/*virtual*/ const char* what() const noexcept
        {
            static const std::string message = "Test failure occured. See related Suite instance for details.";

            return message.c_str();
        }

        //TODO figure out how to have a pointer inside of property
        //TODO revert back to unique_ptr as well eventually
        ResultFailure* pResultFailure;
    };
}}

#endif
