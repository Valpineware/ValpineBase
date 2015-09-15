//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QFile>
#include <QtCore/QDir>
#include <QtCore/QProcess>
#include <QStandardPaths>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "Suite.h"

namespace vbase { namespace test
{
	void Suite::run(bool launchReviewGUI, const QString &testReviewGUIPath)
	{
		QString filepath = QDir::currentPath() + QString("/TestResults/");
		QDir().mkpath(filepath);

		QFile ba(filepath
				 + "Result_"
				 + QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")
				 + ".json");

		if (!ba.open(QFile::WriteOnly | QFile::Text))
		{
			qDebug() << "Unable to write test output";
			qDebug() << filepath;
			qDebug() << ba.errorString();
		}
		else
		{
			run(ba);

			if (launchReviewGUI)
			{
				QString appPath = (testReviewGUIPath == "") ?
									  "TestReviewGUI" : testReviewGUIPath;

				QString path = QFileInfo(ba).absoluteFilePath();
				qDebug() << QProcess::startDetached(appPath, QStringList() << path);
			}
		}
	}


	void Suite::run(QIODevice &outputFileDevice)
    {
        mDateTime_started = QDateTime::currentDateTime();

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
                        testObject->pExecutionTimer().start();
                        metaMethod.invoke(testObject.get(), Qt::DirectConnection);
                        int executionTime = testObject->pExecutionTimer().elapsed();

                        //at this point, the test must have passed since no
                        //exception was thrown
                        auto result = new Result;
                        result->pTestMethod = metaMethod;
                        result->pExecutionTime = executionTime;
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

        mDateTime_finished = QDateTime::currentDateTime();

		exportResults(outputFileDevice);

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
        o.insert("executionTime", QString::number(result->pExecutionTime));

        if (auto *p = dynamic_cast<const ResultFailure*>(result))
        {
            o.insert("status", QString("failed"));
            o.insert("filePath", p->pFilepath()); //TODO fix filepath to filePath
            o.insert("lineNumber", p->pLineNumber());

            QJsonArray messageArray;
            for (const auto &message : p->pMessage())
                messageArray.append(message);

            o.insert("message", messageArray);
        }
        else
        {
            o.insert("status", QJsonValue("passed"));
        }

        return o;
    }


    void Suite::exportResults(QIODevice &ioDevice)
    {
        QJsonObject rootJson;
        rootJson.insert("dateTime_started", mDateTime_started.toString(dateFormat()));
        rootJson.insert("dateTime_finished", mDateTime_finished.toString(dateFormat()));

        QJsonArray classesJsonArray;

        //build array of results
        QMapIterator<QString,QList<Result*>> iter = mResults;

        while (iter.hasNext())
        {
            auto a = iter.next();

            QJsonObject co;
            co.insert("className", QJsonValue(a.key()));

            QJsonArray resultsArray;

            for (const Result *result : a.value())
            {
                resultsArray.append(jsonObjectFromResult(result));
            }

            co.insert("results", resultsArray);
            classesJsonArray.append(co);
        }

        rootJson.insert("results", classesJsonArray);

        QJsonDocument doc(rootJson);
        QTextStream out(&ioDevice);
        out << doc.toJson();
    }
}}
