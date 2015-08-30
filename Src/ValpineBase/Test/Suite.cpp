//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include "Suite.h"

namespace vbase { namespace test
{
    void Suite::run()
    {
        for (TestClassPackageInterface *test : registered())
        {
            std::unique_ptr<Class> defaultInstance(test->makeTestClassInstance());
            const QMetaObject *metaObject = defaultInstance->metaObject();

            for (int i=0; i<metaObject->methodCount(); i++)
            {
                auto metaMethod = metaObject->method(i);

                if (QString(metaMethod.tag()) == "VTEST")
                {
                    std::unique_ptr<Class> testObject(test->makeTestClassInstance());
                    testObject->pHostSuite = this;

                    try
                    {
                        metaMethod.invoke(testObject.get(), Qt::DirectConnection);

                        //at this point, the test must have passed since no
                        //exception was thrown
                        auto result = new Result;
                        result->pTestMethod = metaMethod;
                        post(result);
                    }
                    catch (TestFailureException &tfe)
                    {
                        //swallow the exception
                        //the purpose of throwing the exception from an assert
                        //is to cleanly break out of the test entirely
                        //(even from sub-routines)

                        tfe.pResultFailure->pTestMethod = metaMethod;
                        post(tfe.pResultFailure);
                    }
                }
            }
        }

        //display the results        
        for (auto iter = mResults.begin(); iter != mResults.end(); iter++)
        {
            Result *r = iter->second;

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

        qDebug() << "Finished running all tests";
    }


    void Suite::post(Result *result)
    {
        mResults.emplace(result->pTestMethod().name(), result);
    }
}}
