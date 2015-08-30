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

        Assert(const QString &filepath, int lineNumber) :
            mFilepath(filepath),
            mLineNumber(lineNumber)
        {

        }


        template<typename T>
        void areEq(const QString &verbatimActual, const QString &verbatimExpected,
                   const T &actual, const T &expected) const
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


        void isTrue(const QString &verbatim, bool what) const
        {
            if (!what)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();
                tfe.pResultFailure->pMessage().append(QString("Expected ") + verbatim + " to be true. Got false.");

                throw tfe;
            }
        }


        void isFalse(const QString &verbatim, bool what) const
        {
            if (what)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();
                tfe.pResultFailure->pMessage().append(QString("Expected ") + verbatim + " to be false. Got true.");

                throw tfe;
            }
        }


        void failure(const QString &message) const
        {
            TestFailureException tfe;
            tfe.pResultFailure = makeDefaultResultFailure();
            tfe.pResultFailure->pMessage() << message;

            throw tfe;
        }

    private:
        QString mFilepath;
        int mLineNumber = -1;

        ResultFailure* makeDefaultResultFailure() const
        {
            auto r = new ResultFailure;
            r->pFilepath = mFilepath;
            r->pLineNumber = mLineNumber;

            return r;
        }
    };
}}

#define Assert_Eq(actual, expected) \
    ::vbase::test::Assert(QString(__FILE__), __LINE__).areEq( \
                          QString(#actual), QString(#expected), \
                          actual, expected)

#define Assert_True(what) \
    ::vbase::test::Assert(QString(__FILE__), __LINE__).isTrue( \
                          QString(#what), what)

#define Assert_False(what) \
    ::vbase::test::Assert(QString(__FILE__), __LINE__).isFalse( \
                          QString(#what), what)

#define Assert_Failure(message) \
    ::vbase::test::Assert(QString(__FILE__), __LINE__).failure(message)

#endif
