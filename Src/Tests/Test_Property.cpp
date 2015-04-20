#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Property.h>

using namespace ValpineBase::Test;

#define CLASS Test_Property
TEST_CLASS
{
protected:
};


TEST_CASE(SimpleSetCheck1)
{
    Property<int> pSize = 10;
    ASSERT_EQ(10, pSize);
}


TEST_CASE(SimpleSetCheck2)
{
    Property<bool> pEnabled = true;
    ASSERT_TRUE(pEnabled);
    pEnabled = false;
    ASSERT_FALSE(pEnabled);
}


TEST_CASE(AsMember)
{
    struct A
    {
        Property<float> value;
    };

    A a1;
    a1.value = 40.0f;
    ASSERT_EQ(40.0f, a1.value);
}


TEST_CASE(AsMember2)
{
    struct A
    {
        Property<long> value = 123456;
    };

    A a1;
    ASSERT_EQ(123456, a1.value);
}
