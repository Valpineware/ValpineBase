#include <ValpineBase/Test.h>
#include <ValpineBase/Loadable.h>

using namespace vbase;

class Test_Loadable : public QObject
{
    Q_OBJECT

private slots:
    void simpleCheck()
    {
        Loadable loadable;

        bool didIsLoading = false;
        loadable.pIsLoading.addOnChangedListener([&didIsLoading]
        {
            didIsLoading = true;
        });

        auto f = [&]
        {
            Loadable::Begin b(&loadable);
        };


        QVERIFY(!loadable.pHasLoaded);
        f();
        QVERIFY(loadable.pHasLoaded);
        QVERIFY(didIsLoading);
    }
};

DECLARE_TEST(Test_Loadable)
