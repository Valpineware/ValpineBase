#ifndef _ValpineBase_Test_Test_h
#define _ValpineBase_Test_Test_h

#include <QtTest/QTest>
#include <QList>
#include <QString>
#include <QSharedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QTemporaryFile>
#include <QDir>
#include <QProcess>

namespace _private_Test
{
    typedef QList<QObject*> TestList;

    inline TestList& testList()
    {
        static TestList list;
        return list;
    }


    inline bool findObject(QObject* object)
    {
        TestList& list = testList();

        if (list.contains(object))
            return true;

        foreach (QObject* test, list)
        {
            if (test->objectName() == object->objectName())
                return true;
        }

        return false;
    }


    inline void addTest(QObject* object)
    {
        TestList& list = testList();
        if (!findObject(object))
            list.append(object);
    }


    inline QString run(int argc, char *argv[], bool launchGUI=false)
    {
        int ret = 0;

        QString result;

        foreach (QObject* test, testList())
        {
            QTemporaryFile file;
            file.open();

            QStringList args;
            for (int i=0; i<argc; i++)
                args.append(argv[i]);

            args << "-o" << file.fileName();

            ret += QTest::qExec(test, args);
            result += QString("\r\n") + file.readAll();
        }

        QString timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
        {
            timestamp.replace(" ", "");
            timestamp.replace(":", "");
            timestamp.replace("-", "");
        }

        const QString dirpath = QDir::currentPath() + "/TestResults/";
        const QString filename = QString("run_") + timestamp + ".txt";
        QDir().mkpath(dirpath);
        const QString resultFilepath = dirpath + filename;
        QFile data(resultFilepath);

        if (data.open(QFile::WriteOnly))
        {
            QTextStream out(&data);
            out << result;
        }

        qDebug() << "Test results saved to: " << resultFilepath;

        qDebug() << resultFilepath;

        if (launchGUI && !QProcess::startDetached("QtTestReviewGUI", QStringList() << resultFilepath))
        {
            qDebug() << "QtTestReviewGUI is not installed in the system path!";
        }

        for (const QString &line : result.split("\r\n"))
            qDebug() << line;

        return resultFilepath;
    }

    template <class T>
    class Test
    {
    public:
        QSharedPointer<T> child;

        Test(const QString& name) : child(new T)
        {
            child->setObjectName(name);
            _private_Test::addTest(child.data());
        }
    };
}

#define DECLARE_TEST(className) static _private_Test::Test<className> t_##className(#className);

#define QCOMPARE_WSTRINGS(a, b) QCOMPARE(QString::fromStdWString(a), QString::fromStdWString(b))

#endif
