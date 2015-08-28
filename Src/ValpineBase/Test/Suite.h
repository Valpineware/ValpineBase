#ifndef vbase_test_Suite_h
#define vbase_test_Suite_h

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>

#include <memory>

namespace vbase { namespace test
{
    class Suite
    {
    public:
        void run();

        template<typename T>
        struct TestAdder
        {
            TestAdder()
            {
                mRegistered.append(new TestClassPackage<T>);
            }
        };

    private:
        class TestClassPackageInterface
        {
        public:
            virtual void runTests() = 0;
        };


        // TODO convert to private implementation
        template<typename T>
        class TestClassPackage : public TestClassPackageInterface
        {
        public:
            virtual void runTests() override
            {
                const QMetaObject *metaObject = T().metaObject();

                for (int i=0; i<metaObject->methodCount(); i++)
                {
                    auto method = metaObject->method(i);

                    if (QString(method.tag()) == "VTEST")
                    {
                        T testObject;

                        try
                        {
                            method.invoke(&testObject, Qt::DirectConnection);
                        }
                        catch (std::exception &e)
                        {
                            qWarning() << metaObject->className() << "::"
                                       << method.name() << "() failure.";
                        }
                    }
                }
            }
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
