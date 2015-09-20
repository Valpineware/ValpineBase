//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Class_h
#define vbase_test_Class_h

#include <QObject>
#include <QtCore/QElapsedTimer>
#include <QtCore/QString>

#include <ValpineBase/Property.h>

namespace vbase { namespace test
{
    class Suite;

    class Class : public QObject
    {
        Q_OBJECT

    public:
        Property<Suite*> pHostSuite;
        Property<QElapsedTimer> pExecutionTimer;
    };
}}

#endif
