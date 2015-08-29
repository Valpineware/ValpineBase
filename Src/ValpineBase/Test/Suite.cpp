//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include "Suite.h"
#include <iostream>
#include <QThread>

namespace vbase { namespace test
{
    void Suite::run()
    {
        qDebug() << "Running tests on thread id " << static_cast<void*>(QThread::currentThread());

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
                        auto result = std::make_unique<Result>();
                        result->pTestMethod = metaMethod;
                        this->post(std::move(result));
                    }
                    catch (const TestFailureException &e)
                    {
                        //swallow the exception
                        //the purpose of throwing the exception from an assert
                        //is to cleanly break out of the test entirely
                        //(even from sub-routines)
                    }
                }
            }
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

        qDebug() << "Finished running all tests";
    }


    void Suite::post(std::unique_ptr<Result> result)
    {
        mResults.emplace(result->pTestMethod().typeName(), std::move(result));
    }
}}
