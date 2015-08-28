//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include "Suite.h"

namespace vbase { namespace test
{
    QList<Suite::TestClassPackageInterface*> Suite::mRegistered;

    void Suite::run()
    {
        for (TestClassPackageInterface *test : mRegistered)
        {
            test->runTests();
        }
    }
}}
