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
            test->runTests(this);
        }

        //display the results        
        for (auto iter = mResults.begin(); iter != mResults.end(); iter++)
        {
            Result *r = iter->second.get();

            qDebug() << r->pTestMethod().name();

            //failure?
            if (ResultFailure *rf = dynamic_cast<ResultFailure*>(r))
            {
                qDebug() << "\t" << "FAILED";
                qDebug() << "\t" << rf->pMessage();
                qDebug() << "\t" << rf->pFilepath();
                qDebug() << "\t" << rf->pLineNumber();
            }
            else
                qDebug() << "\t" << "PASSED";
        }
    }


    void Suite::post(std::unique_ptr<Result> result)
    {
        mResults.emplace(result->pTestMethod().typeName(), std::move(result));
    }
}}
