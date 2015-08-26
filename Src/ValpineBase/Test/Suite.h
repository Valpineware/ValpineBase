#ifndef vbase_test_Suite_h
#define vbase_test_Suite_h

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QList>

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
                Suite::mRegistered.append(new T);
            }
        };

    private:
        static QList<QObject*> mRegistered;
    };

#ifndef Q_MOC_RUN
// define the tag text
#  define vTEST
#endif

#define ADD_TESTCLASS(name) \
    static vbase::test::Suite::TestAdder<name> t_##name;
}}

#endif
