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
        void Eq(const QString &verbatimActual,
                       const QString &verbatimExpected,
                       const T &actual, const T &expected)
        {
            if (actual != expected)
            {
                auto r = makeDefaultResultFailure();
                r->pMessage().append(verbatimActual + " != " + verbatimExpected);
                r->pMessage().append(QString("Expected: ") + expected);
                r->pMessage().append(QString("Actual: ") + actual);

                //TODO find a way to remove the need for std::move (if possible)
                postResult(std::move(r));

               throw TestFailureException();
            }
        }


        template<typename T>
        void True(const QString &verbatim, const T &what)
        {
            if (!what)
            {
                auto r = makeDefaultResultFailure();
                postResult(std::move(r));

                throw TestFailureException();
            }
        }

    private:
        Class *mTestClass = nullptr;
        QString mFilepath;
        int mLineNumber = -1;

        std::unique_ptr<ResultFailure> makeDefaultResultFailure()
        {
            auto r = std::make_unique<ResultFailure>();
            r->pFilepath = mFilepath;
            r->pLineNumber = mLineNumber;

            return r;
        }


        void postResult(std::unique_ptr<ResultFailure> &&result)
        {
            mTestClass->pHostSuite()->post(std::move(result));
        }
    };
}}

#define Assert_Eq(actual, expected) \
    ::vbase::test::Assert(this, QString(__FILE__), __LINE__).Eq( \
                                QString(#actual), QString(#expected), \
                                actual, expected)

#define Assert_True(what) \
    ::vbase::test::Assert(this, QString(__FILE__), __LINE__).True( \
                                    QString(#what), what)

#endif
