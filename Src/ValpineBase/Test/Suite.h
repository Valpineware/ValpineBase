//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Suite_h
#define vbase_test_Suite_h

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QPair>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>

#include <map>
#include <memory>

#include <ValpineBase/System.h>
#include <ValpineBase/Test/Result.h>

namespace vbase { namespace test
{
    class Suite
    {
    public:
        void run();
        void post(std::unique_ptr<Result> result);

        template<typename T>
        struct TestAdder
        {
            TestAdder()
            {
                mRegistered.append(new TestClassPackage<T>);
            }
        };

    private:
        /**
         * @brief mResults
         */
        std::map<QString, std::unique_ptr<Result>> mResults;

    private:
        class TestClassPackageInterface
        {
        public:
            virtual void runTests(Suite *suite) = 0;
        };


        // TODO convert to private implementation
        template<typename T>
        class TestClassPackage : public TestClassPackageInterface
        {
        public:
            virtual void runTests(Suite *suite) override
            {
                const QMetaObject *metaObject = T().metaObject();

                for (int i=0; i<metaObject->methodCount(); i++)
                {
                    auto metaMethod = metaObject->method(i);

                    if (QString(metaMethod.tag()) == "VTEST")
                    {
                        T testObject;
                        testObject.pHostSuite = suite;

                        try
                        {
                            metaMethod.invoke(&testObject, Qt::DirectConnection);

                            auto result = std::make_unique<Result>();
                            result->pTestMethod = metaMethod;
                            suite->post(std::move(result));
                        }
                        catch (const TestFailureException &e)
                        {
                            // swallow the exception
                            // the purpose of throwing the exception from an assert
                            // is to cleanly break out of the test entirely
                            // (even from sub-routines)
                        }
                    }
                }
            }

        private:
//            void printAssertException(const QMetaObject &metaObject,
//                                      const QMetaMethod &metaMethod,
//                                      const AssertException &e)
//            {
//                System::warn() << "FAILURE: " << metaObject.className() << "::" << metaMethod.name() << "() ";

//                System::warn() << "{";

//                for (const QString &line : e.pMessage())
//                    System::warn() << "\t" << line;


//                System::warn(true) << "} filepath=" << e.pFilepath() << " lineNumber=" << QString::number(e.pLineNumber);
//            }
        };


        static QList<TestClassPackageInterface*> mRegistered;
    };

#ifndef Q_MOC_RUN
// define the tag text
#  define VTEST
#endif

#define ADD_TESTCLASS(name) \
    static vbase::test::Suite::TestAdder<name> t_##name;
}}

#endif
