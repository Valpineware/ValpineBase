#include <ValpineBase/Test.h>
#include <ValpineBase/Property.h>

#include <functional>

using namespace vbase;

class Test_Property_Operators : public QObject
{
private:
    struct A
    {
        A() = default;
        A(int v) : value(v) {}

        bool operator==(const A &rhs) const { return rhs.value == this->value; }

        A& operator+=(const A &rhs)
        {
            value += rhs.value;
            return *this;
        }


        A operator+(const A &rhs) const { return A(value+rhs.value); }
        A operator-(const A &rhs) const { return A(value-rhs.value); }
        A operator*(const A &rhs) const { return A(value*rhs.value); }
        A operator/(const A &rhs) const { return A(value/rhs.value); }
        A operator%(const A &rhs) const { return A(value%rhs.value); }


        int value = 0;
    };

    typedef Property<A> (Property<A>::* memf)(const A&);

    void assertCompoundAssignment(memf f, const A &amount, const A &expected)
    {
        struct Test
        {
            Property_Set(A, pA, 100,
            {
                pA.raw() = _newValue;
                setCalled = true;
            })

            bool setCalled = false;
        } t;

        (t.pA.*f)(amount);

        QVERIFY(t.setCalled);
        QCOMPARE(t.pA, expected);
    }

private slots:
    void Operator_AddAssign()
    {
        assertCompoundAssignment(&Property<A>::operator+=, 50, 150);
    }

    void Operator_SubtractAssign()
    {
        assertCompoundAssignment(&Property<A>::operator-=, 50, 50);
    }

    void Operator_MultiplyAssign()
    {
        assertCompoundAssignment(&Property<A>::operator*=, 2, 200);
    }

    void Operator_DivideAssign()
    {
        assertCompoundAssignment(&Property<A>::operator/=, 2, 50);
    }

    void Operator_ModuloAssign()
    {
        assertCompoundAssignment(&Property<A>::operator%=, 3, 1);
    }
};

DECLARE_TEST(Test_Property_Operators)
