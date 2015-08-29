//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Class_h
#define vbase_test_Class_h

#include <QObject>

#include <ValpineBase/Property.h>
#include <ValpineBase/Test/Suite.h>

namespace vbase { namespace test
{
    class Class : public QObject
    {
        Q_OBJECT

    public:
        Property<Suite*> pHostSuite;
    };
}}

#endif
