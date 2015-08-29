//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Assert_h
#define vbase_test_Assert_h

#include <exception>
#include <QString>

#include <ValpineBase/Test/Result.h>
#include <ValpineBase/Test/Class.h>
#include <ValpineBase/Test/Suite.h>

namespace vbase { namespace test
{
    class Assert
    {
    public:
        template<typename T>
        static void Eq(Class *testClass, const QString &filepath, int lineNumber,
                       const QString &verbatimActual,
                       const QString &verbatimExpected,
                       const T &actual, const T &expected)
        {
            if (actual != expected)
            {
                auto result = std::make_unique<ResultFailure>();
                testClass->pHostSuite()->post(std::move(result));
//                AssertException e;
//                e.pMessage().append(verbatimActual + " != " + verbatimExpected);
//                e.pMessage().append(QString("Expected: ") + expected);
//                e.pMessage().append(QString("Actual: ") + actual);

//                e.pFilepath = filepath;
//                e.pLineNumber = lineNumber;


               throw TestFailureException();
            }
        }


        template<typename T>
        static void True(Class *testClass, const QString &filepath, int lineNumber,
                         const QString &verbatim, const T &what)
        {
            if (!what)
            {
                auto result = std::make_unique<ResultFailure>();
                testClass->pHostSuite()->post(std::move(result));

                throw TestFailureException();

//                AssertException e;
//                e.pMessage().append("");
//                e.pFilepath = filepath;
//                e.pLineNumber = lineNumber;


//                throw e;
            }
        }
    };
}}

#define Assert_Eq(actual, expected) \
    ::vbase::test::Assert::Eq(this, QString(__FILE__), __LINE__, \
                                QString(#actual), QString(#expected), \
                                actual, expected)

#define Assert_True(what) \
    ::vbase::test::Assert::True(this, QString(__FILE__), __LINE__, \
                                    QString(#what), what)

#endif
