#include <QtCore/QDebug>
#include <QtCore/QMetaMethod>

#include "Suite.h"

namespace vbase { namespace test
{
    QList<QObject*> Suite::mRegistered;

    void Suite::run()
    {
        qDebug() << "Discovering tests...";

        for (QObject *test : mRegistered)
        {
            const auto *metaObject = test->metaObject();
            qDebug() << "TestClass " << metaObject->className();


            QList<QMetaMethod> testMethods;

            for (int i=0; i<metaObject->methodCount(); i++)
            {
                auto method = metaObject->method(i);
                if (QString(method.tag()) == "vTEST")
                {
                    testMethods.append(method);
                    qDebug() << "  Case: " << method.name();
                }
            }
        }
    }
}}
