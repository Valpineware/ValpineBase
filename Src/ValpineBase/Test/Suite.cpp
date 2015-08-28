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
