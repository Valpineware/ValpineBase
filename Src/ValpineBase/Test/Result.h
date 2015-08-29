//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_AssertException_h
#define vbase_test_AssertException_h

#include <exception>
#include <QString>
#include <QStringList>
#include <QMetaMethod>

#include <ValpineBase/Property.h>

namespace vbase { namespace test
{
    /**
     * Indicates a test failure due to an assert being triggered.
     */
    class TestFailureException : public std::exception
    {
    public:
        virtual const char* what() const noexcept override
        {
            static const std::string message = "Test failure occured. See related Suite instance for details.";

            return message.c_str();
        }
    };


    class Result
    {
    public:
        virtual ~Result() {}
        Property<QMetaMethod> pTestMethod;
    };


    class ResultFailure : public Result
    {
    public:
        Property<QStringList> pMessage;
        Property<QString> pFilepath;
        Property<int> pLineNumber = -1;
    };
}}

#endif
