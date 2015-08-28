//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_AssertException_h
#define vbase_test_AssertException_h

#include <exception>
#include <QString>
#include <QStringList>

#include <ValpineBase/Property.h>

namespace vbase { namespace test
{
    /**
     * Indicates a test failure due to an assert being triggered.
     */
    class AssertException
    {
    public:
        Property<QStringList> pMessage;
        Property<QString> pFilepath;
        int pLineNumber = -1;
    };
}}

#endif
