#ifndef _ValpineBase_Test_Test_h
#define _ValpineBase_Test_Test_h

#include <QTest>
#include <QList>
#include <QString>
#include <QSharedPointer>
#include <QTextStream>
#include <QDateTime>
#include <QTemporaryFile>
#include <QDir>

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


    inline QString run(int argc, char *argv[])
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
        QFile data(dirpath + filename);

        if (data.open(QFile::WriteOnly))
        {
            QTextStream out(&data);
            out << result;
        }

        return dirpath + filename;
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

#endif
