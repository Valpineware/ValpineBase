#include <QObject>

#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Property.h>
#include <ValpineBase/System.h>

using namespace vbase;

class Test_Property : public QObject
{
    Q_OBJECT

private slots:
    /*
     * General construction and assignment
     */

    void SimpleSetCheck1()
    {
        Property<int> pSize = 10;
        QCOMPARE(pSize(), 10);
    }


    void SimpleSetCheck2()
    {
        Property<bool> pEnabled = true;
        QVERIFY(pEnabled);
        pEnabled = false;
        QVERIFY(!pEnabled);
    }


    void AsMember1()
    {
        struct A
        {
            Property<float> value;
        };

        A a1;
        a1.value = 40.0f;
        QCOMPARE(a1.value(), 40.0f);
    }


    void AsMember2()
    {
        struct A
        {
            Property<long> value = 123456;
        };

        A a1;
        QCOMPARE(a1.value(), 123456L);
    }


    void Assign()
    {
        Property<int> size = 35;
        QCOMPARE(size(), 35);
        size = 40;
        QCOMPARE(size(), 40);
    }


    void ImplicitUse()
    {
        Property<float> amount = 77.4f;

        struct
        {
            float implicit;
            void use(float a) { implicit = a; }
        } usage;

        usage.use(amount);
        QCOMPARE(usage.implicit, 77.4f);
    }


    void InitializationFromOtherProperty()
    {
        Property<int> pA = 10;
        Property<int> pB = pA;
        pA = 5;

        QCOMPARE(pA(), 5);
        QCOMPARE(pB(), 10);
    }


    void AssignmentFromOtherProperty()
    {

        struct Test
        {
            bool setterCalled = true;

            Property_Set(int, pA, 5, {
                pA.raw() = _newValue;
                setterCalled = true;
            })

            Property<int> pB = 60;
        } t1;

        t1.pA = t1.pB;
        t1.pB = 7;

        QCOMPARE(t1.pA(), 60);
        QCOMPARE(t1.pB(), 7);
        QVERIFY(t1.setterCalled);
    }


    /*
     * Custom setters and getters
     */

    void CustomSetter1()
    {
        Property<int> pSize(0, [&](const int &_new)
        {
            pSize.raw() = std::max(std::min(100, _new), 0);
        });

        pSize = 400;
        QCOMPARE(pSize(), 100);
    }


    void CustomSetter2()
    {
        Property<int> pSize(0, [&](const int &_new)
        {
            pSize.raw() = std::max(std::min(850, _new), 0);
        });

        pSize = 1238;
        QCOMPARE(pSize(), 850);
        pSize = 430;
        QCOMPARE(pSize(), 430);
        pSize = 853;
        QCOMPARE(pSize(), 850);
    }


    void CustomSetter_NoDefaultValue()
    {
        Property<int> pSize([&](const int &_new)
        {
            pSize.raw() = std::max(std::min(5, _new), 0);
        });

        pSize = 49;
        QCOMPARE(pSize(), 5);
        pSize = 4;
        QCOMPARE(pSize(), 4);
    }


    void CustomGetter1()
    {
        bool getCalled = false;

        Property<int> pSize(0, [&]() -> int
        {
            getCalled = true;
            return pSize.raw() + 100;
        });

        QVERIFY(!getCalled);
        pSize = 1234;
        QCOMPARE(pSize(), 1334);
    }


    void CustomGetter2()
    {
        struct Test
        {
            Property<int> pSize = Property<int>(0, [this]() -> int
            {
                return pSize.raw() + 100;
            });
        } t1;

        QCOMPARE(t1.pSize(), 100);
    }


    void CustomSetterAndGetter()
    {
        struct Test
        {
            bool setCalled = false;
            bool getCalled = false;

            Property<int> pAmount = Property<int>(14,
                    [this](const int &_new) {
                        setCalled = true;
                        pAmount.raw() = _new+5;
                    },

                    [this]() -> int {
                        getCalled = true;
                        return pAmount.raw()+1;
                    });
        } t1;

        QVERIFY(!t1.setCalled);
        QVERIFY(!t1.getCalled);

        QCOMPARE(t1.pAmount(), 15);
        t1.pAmount = 15;
        QCOMPARE(t1.pAmount(), 21);
        QCOMPARE(t1.pAmount.raw(), 20);
    }



    /*
     * Custom setters and getters with simplified macros
     */

    void MacroCustomSetter()
    {
        struct Test
        {
            Property_Set(int, pAmount, 100,
            {
                pAmount.raw() = _newValue-4;
            })
        } t1;

        t1.pAmount = 80;
        QCOMPARE(t1.pAmount(), 76);
    }


    void MacroCustomGetter()
    {
        struct Test
        {
            Property_Get(int, pAmount, 80,
            {
                return pAmount.raw() + 6;
            })
        } t1;

        QCOMPARE(t1.pAmount(), 86);
    }


    void MacroCustomSetterGetter()
    {
        struct Test
        {
            Property_SetGet(double, pAmount, 100.0,
            {
                pAmount.raw() = _newValue + 3.0;
            },
            {
                return pAmount.raw() - 4.0;
            })
        } t1;

        QCOMPARE(t1.pAmount.raw(), 100.0);
        t1.pAmount = 96.0;
        QCOMPARE(t1.pAmount(), 95.0);
        QCOMPARE(t1.pAmount.raw(), 99.0);
    }


    /*
     * Custom onChanged listeners
     */

    void CustomOnChangedListener()
    {
        Property<bool> pEnabled = true;
        bool wasNotified1 = false;
        bool wasNotified2 = false;

        pEnabled.addOnChangedListener([&]{ wasNotified1 = true; });
        pEnabled.addOnChangedListener([&]{ wasNotified2 = true; });

        pEnabled = !pEnabled;

        QVERIFY2(wasNotified1, "On changed listener 1 was not notified");
        QVERIFY2(wasNotified2, "On changed listener 2 was not notified");
    }


    void CustomTypeUsage()
    {
        struct Test
        {
            Property_Set(QString, pName, QString("D"),
            {
                pName.raw() = _newValue;
                setCalled = true;
            })

            bool setCalled = false;
        } t;

        t.pName += "an";
        QVERIFY(t.setCalled);
    }


    /*
     * Extra constraints
     */
    void DoNotCallSetterOnInitialization()
    {
        //this is so that when a setter uses another property in the same
        //class that the initialization order doesn't mess up when one
        //property is initialized before the other one it accesses is
        //initialized

        struct Test
        {
            Property_Set(int, pAmount, 80,
            {
                pAmount.raw() = _newValue;
                pIsModdified = true;
            })

            Property<bool> pIsModdified = false;
        } t1;

        QCOMPARE(t1.pAmount(), 80);
        QVERIFY(!t1.pIsModdified);

        t1.pAmount = 45;

        QCOMPARE(t1.pAmount(), 45);
        QVERIFY(t1.pIsModdified);
    }
};

DECLARE_TEST(Test_Property)
