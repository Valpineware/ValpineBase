#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Property.h>

using namespace ValpineBase::Test;

#define CLASS Test_Property
TEST_CLASS
{
protected:
};

/*
 * General construction and assignment
 */

TEST_CASE(SimpleSetCheck1)
{
    Property<int> pSize = 10;
	Assert::Eq(10, pSize);
}


TEST_CASE(SimpleSetCheck2)
{
    Property<bool> pEnabled = true;
	Assert::True(pEnabled);
    pEnabled = false;
	Assert::False(pEnabled);
}


TEST_CASE(AsMember)
{
    struct A
    {
        Property<float> value;
    };

    A a1;
    a1.value = 40.0f;
	Assert::Eq(40.0f, a1.value);
}


TEST_CASE(AsMember2)
{
    struct A
    {
        Property<long> value = 123456;
    };

    A a1;
	Assert::Eq(123456, a1.value);
}


TEST_CASE(Assign)
{
	Property<int> size = 35;
	Assert::Eq(35, size);
	size = 40;
	Assert::Eq(40, size);
}


TEST_CASE(ImplicitUse)
{
	Property<float> amount = 77.4f;

	struct
	{
		float implicit;
		void use(float a) { implicit = a; }
	} usage;

	usage.use(amount);
	Assert::Eq(77.4f, usage.implicit);
}


/*
 * Custom setters and getters
 */

TEST_CASE(CustomSetter1)
{
	Property<int> pSize(277, [](const int &_new, int &_value)
	{
		_value = std::max(std::min(100, _new), 0);
	});

	Assert::Eq(100, pSize);
}


TEST_CASE(CustomSetter2)
{
	Property<int> pSize(1501, [](const int &_new, int &_value)
	{
		_value = std::max(std::min(850, _new), 0);
	});

	Assert::Eq(850, pSize);
	pSize = 430;
	Assert::Eq(430, pSize);
	pSize = 853;
	Assert::Eq(850, pSize);
}



TEST_CASE(CustomSetter_NoDefaultValue)
{
	Property<int> pSize([](const int &_new, int &_value)
	{
		_value = std::max(std::min(5, _new), 0);
	});

	pSize = 49;
	Assert::Eq(5, pSize);
	pSize = 4;
	Assert::Eq(4, pSize);
}


