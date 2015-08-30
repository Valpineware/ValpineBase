//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QFile>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

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
                        post(metaObject->className(), result);
                    }
                    catch (TestFailureException &tfe)
                    {
                        //swallow the exception
                        //the purpose of throwing the exception from an assert
                        //is to cleanly break out of the test entirely
                        //(even from sub-routines)

                        tfe.pResultFailure->pTestMethod = metaMethod;
                        post(metaObject->className(), tfe.pResultFailure);
                    }
                }
            }
        }

        //display the results        
//        for (auto iter = mResults.begin(); iter != mResults.end(); iter++)
//        {
//            Result *r = iter->;

//            qDebug() << r->pTestMethod().name();

//            //failure?
//            if (ResultFailure *rf = dynamic_cast<ResultFailure*>(r))
//            {
//                qDebug() << "\t" << "FAILED";
//                qDebug() << "\t" << rf->pMessage();
//                qDebug() << "\t" << rf->pFilepath();
//                qDebug() << "\t" << rf->pLineNumber();
//            }
//            else
//                qDebug() << "\t" << "PASSED";

//        }

        QFile ba("C:/dump_115124141324.txt");
        if (!ba.open(QIODevice::WriteOnly | QIODevice::Text))
            qDebug() << "Unable to open";
        else
            exportResults(ba);

        qDebug() << "Finished running all tests";
    }


    void Suite::post(const QString &className, Result *result)
    {
        if (mResults.find(className) == mResults.end())
            mResults.insert(className, QList<Result*>());

        mResults.find(className).value().append(result);
    }


    QJsonObject jsonObjectFromResult(const Result *result)
    {
        QJsonObject o;
        o.insert("name", QString(result->pTestMethod().name()));

        if (auto *p = dynamic_cast<const ResultFailure*>(result))
        {
            o.insert("status", QString("failed"));
            o.insert("filePath", p->pFilepath()); //TODO fix filepath to filePath
            o.insert("lineNumber", p->pLineNumber());
        }
        else
        {
            o.insert("status", QJsonValue("passed"));
        }

        return o;
    }


    void Suite::exportResults(QIODevice &ioDevice)
    {
        QJsonArray classes;

        //build array of results
        QMapIterator<QString,QList<Result*>> iter = mResults;

        while (iter.hasNext())
        {
            auto a = iter.next();

            qDebug() << a.key();
            QJsonObject co;
            co.insert("className", QJsonValue(a.key()));

            QJsonArray resultsArray;

            for (const Result *result : a.value())
            {
                qDebug() << "\t" << result->pTestMethod().name();
                resultsArray.append(jsonObjectFromResult(result));
            }

            co.insert("results", resultsArray);
            classes.append(co);
        }

        QJsonDocument doc(classes);

        QFile data(QStandardPaths::DesktopLocation + "/file.txt");

        if (data.open(QFile::WriteOnly))
        {
            QTextStream out(&data);
            out << doc.toJson();
        }
        else
            qDebug() << "Goodbye";
    }
}}
