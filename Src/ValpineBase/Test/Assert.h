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
        Assert() = delete;

        Assert(Class *testClass, const QString &filepath, int lineNumber) :
            mTestClass(testClass),
            mFilepath(filepath),
            mLineNumber(lineNumber)
        {
        }


        template<typename T>
        void areEq(const QString &verbatimActual, const QString &verbatimExpected,
                const T &actual, const T &expected)
        {
            if (actual != expected)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();
                tfe.pResultFailure->pMessage().append(verbatimActual + " != " + verbatimExpected);
                tfe.pResultFailure->pMessage().append(QString("Expected: ") + expected);
                tfe.pResultFailure->pMessage().append(QString("Actual: ") + actual);

               throw tfe;
            }
        }


        template<typename T>
        void isTrue(const QString &verbatim, const T &what)
        {
            if (!what)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();

                throw tfe;
            }
        }


        void failure(const QString &message)
        {
            TestFailureException tfe;
            tfe.pResultFailure = makeDefaultResultFailure();
            tfe.pResultFailure->pMessage() << message;

            throw tfe;
        }

    private:
        Class *mTestClass = nullptr;
        QString mFilepath;
        int mLineNumber = -1;

        ResultFailure* makeDefaultResultFailure()
        {
            auto r = new ResultFailure;
            r->pFilepath = mFilepath;
            r->pLineNumber = mLineNumber;

            return r;
        }


        void postResult(ResultFailure *result)
        {
            mTestClass->pHostSuite()->post(result);
        }
    };
}}

#define Assert_Eq(actual, expected) \
    ::vbase::test::Assert(this, QString(__FILE__), __LINE__).areEq( \
                                QString(#actual), QString(#expected), \
                                actual, expected)

#define Assert_True(what) \
    ::vbase::test::Assert(this, QString(__FILE__), __LINE__).isTrue( \
                                    QString(#what), what)

#define Assert_Failure(message) \
    ::vbase::test::Assert(this, QString(__FILE__), __LINE__).failure(message)

#endif
