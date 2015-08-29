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
#include <ValpineBase/Test/Class.h>

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
                registered().append(new TestClassPackage<T>);
            }
        };

    private:
        /**
         * @brief mResults
         */
        std::map<QString, std::unique_ptr<Result>> mResults;

        class TestClassPackageInterface
        {
        public:
            virtual Class *makeTestClassInstance() = 0;
        };


        // TODO convert to private implementation
        template<typename T>
        class TestClassPackage : public TestClassPackageInterface
        {
        public:            
            virtual Class *makeTestClassInstance()
            {
                return new T;
            }
        };

        static QList<TestClassPackageInterface*>& registered()
        {
            static QList<TestClassPackageInterface*> reg;
            return reg;
        }
    };

#ifndef Q_MOC_RUN
// define the tag text
#  define VTEST
#endif

#define ADD_TESTCLASS(name) \
    static vbase::test::Suite::TestAdder<name> t_##name;
}}

#endif
