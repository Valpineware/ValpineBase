#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Property.h>

#include <glm/glm.hpp>

#include <functional>

using namespace vbase;

#define CLASS Test_Property_Operators
TEST_CLASS
{
protected:
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

        ASSERT_TRUE(t.setCalled);
        ASSERT_EQ(expected, t.pA);
    }
};


TEST_CASE(Operator_AddAssign)
{
    assertCompoundAssignment(&Property<A>::operator+=, 50, 150);
}

TEST_CASE(Operator_SubtractAssign)
{
    assertCompoundAssignment(&Property<A>::operator-=, 50, 50);
}

TEST_CASE(Operator_MultiplyAssign)
{
    assertCompoundAssignment(&Property<A>::operator*=, 2, 200);
}

TEST_CASE(Operator_DivideAssign)
{
    assertCompoundAssignment(&Property<A>::operator/=, 2, 50);
}

TEST_CASE(Operator_ModuloAssign)
{
    assertCompoundAssignment(&Property<A>::operator%=, 3, 1);
}
