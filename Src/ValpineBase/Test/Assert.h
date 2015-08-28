//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Assert_h
#define vbase_test_Assert_h

#include <exception>
#include <QString>

#include <ValpineBase/Property.h>
#include <ValpineBase/Test/AssertException.h>

namespace vbase { namespace test
{
    class Assert
    {
    public:
        template<typename T>
        static void Eq(const QString &filepath, int lineNumber,
                       const T &actual, const T &expected)
        {
            if (actual != expected)
            {
                AssertException e;
                e.pMessage().append(QString("Compared ") + typeid(T).name() + " are not equal");
                e.pFilepath = filepath;
                e.pLineNumber = lineNumber;


                throw e;
            }
        }
    };

#define Assert_Eq(actual, expected) ::vbase::test::Assert::Eq(QString(__FILE__), __LINE__, actual, expected)

}}

#endif
