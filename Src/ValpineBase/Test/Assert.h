//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Assert_h
#define vbase_test_Assert_h

#include <exception>
#include <QString>

#include <ValpineBase/Test/AssertException.h>

namespace vbase { namespace test
{
    class Assert
    {
    public:
        template<typename T>
        static void Eq(const QString &filepath, int lineNumber,
                       const QString &verbatimActual,
                       const QString &verbatimExpected,
                       const T &actual, const T &expected)
        {
            if (actual != expected)
            {
                AssertException e;
                e.pMessage().append(verbatimActual + " != " + verbatimExpected);
                e.pMessage().append(QString("Expected: ") + expected);
                e.pMessage().append(QString("Actual: ") + actual);

                e.pFilepath = filepath;
                e.pLineNumber = lineNumber;


                throw e;
            }
        }


        template<typename T>
        static void True(const QString &filepath, int lineNumber,
                         const QString &verbatim, const T &what)
        {
            if (!what)
            {
                AssertException e;
                e.pMessage().append("");
                e.pFilepath = filepath;
                e.pLineNumber = lineNumber;


                throw e;
            }
        }
    };
}}

#define Assert_Eq(actual, expected) \
    ::vbase::test::Assert::Eq(QString(__FILE__), __LINE__, \
                                QString(#actual), QString(#expected), \
                                actual, expected)

#define Assert_True(what) \
    ::vbase::test::Assert::True(QString(__FILE__), __LINE__, \
                                    QString(#what), what)

#endif
