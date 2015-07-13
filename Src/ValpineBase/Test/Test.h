#ifndef _ValpineBase_Test_Test_h
#define _ValpineBase_Test_Test_h

#include <QTest>
#include <QList>
#include <QString>
#include <QSharedPointer>

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


    inline int run(int argc, char *argv[])
    {
        int ret = 0;

        foreach (QObject* test, testList())
        {
            ret += QTest::qExec(test, argc, argv);
        }

        return ret;
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
};

#define DECLARE_TEST(className) static _private_Test::Test<className> t_##className(#className);

#endif
