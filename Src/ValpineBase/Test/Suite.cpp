//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QFile>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
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
            qDebug() << "Full path is " << QFileInfo(ba).absoluteFilePath();

			run(ba);

			if (launchReviewGUI)
			{
				QString appPath = (testReviewGUIPath == "") ?
									  "TestReviewGUI" : testReviewGUIPath;

				QString path = QFileInfo(ba).absoluteFilePath();
				qDebug() << QProcess::startDetached(appPath, QStringList() << path);
			}
			else
			{
				qDebug() << "Not running GUI review tool";
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
					testObject->hostSuite = this;

                    try
                    {
						testObject->executionTimer.start();
                        metaMethod.invoke(testObject.get(), Qt::DirectConnection);
						int executionTime = testObject->executionTimer.elapsed();

                        //at this point, the test must have passed since no
                        //exception was thrown
                        auto result = new Result;
                        result->testMethod = metaMethod;
                        result->executionTime = executionTime;
                        post(metaObject->className(), result);
                    }
                    catch (TestAssertException &tfe)
                    {
                        //swallow the exception
                        //the purpose of throwing the exception from an assert
                        //is to cleanly break out of the test entirely
                        //(even from sub-routines)

                        tfe.pResultFailure->testMethod = metaMethod;
                        post(metaObject->className(), tfe.pResultFailure);
                    }
                }
            }
        }

        mDateTime_finished = QDateTime::currentDateTime();

		exportResults(outputFileDevice);

        qDebug() << "Finished running all tests";
    }


    void Suite::printResults()
    {
        //QString, QList<Result*>

        QMapIterator<QString, QList<Result*>> iter(mResults);
        while (iter.hasNext())
        {
            auto item = iter.next();

            for (Result *result : item.value())
            {
                if (auto failure = dynamic_cast<ResultFailure*>(result))
                {
					qDebug() << "FAILED: [" << failure->testMethod.name() << "] - - - - - - - -";

					for (auto line : failure->message)
                    {
                        qDebug() << "      -" << line;
                    }

					qDebug() << "  At " << failure->filepath;
					qDebug() << "  Line " << failure->lineNumber;

                    qDebug() << "";
                }
            }
        }
    }


	void Suite::cleanOldResults(int maxAgeSeconds)
	{
		QDirIterator iter("./TestResults", QDirIterator::Subdirectories);
		while (iter.hasNext())
		{
			QFileInfo fi(iter.next());

			if (fi.lastModified().secsTo(QDateTime::currentDateTime())
				> maxAgeSeconds)
			{
				QFile::remove(fi.absoluteFilePath());
			}
		}
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
		o.insert("name", QString(result->testMethod.name()));
        o.insert("executionTime", QString::number(result->executionTime));

        if (auto *p = dynamic_cast<const ResultFailure*>(result))
        {
            o.insert("status", QString("failed"));
			o.insert("filePath", p->filepath); //TODO fix filepath to filePath
			o.insert("lineNumber", p->lineNumber);

            QJsonArray messageArray;
			for (const auto &message : p->message)
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

