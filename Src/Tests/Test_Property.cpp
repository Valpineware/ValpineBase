#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Property.h>

using namespace vbase;

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


TEST_CASE(Assign)
{
	Property<int> size = 35;
	ASSERT_EQ(35, size);
	size = 40;
	ASSERT_EQ(40, size);
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
	ASSERT_EQ(77.4f, usage.implicit);
}


TEST_CASE(InitializationFromOtherProperty)
{
	Property<int> pA = 10;
	Property<int> pB = pA;
	pA = 5;

	ASSERT_EQ(5, pA);
	ASSERT_EQ(10, pB);
}


TEST_CASE(AssignmentFromOtherProperty)
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

	ASSERT_EQ(60, t1.pA);
	ASSERT_EQ(7, t1.pB);
	ASSERT_TRUE(t1.setterCalled);
}


/*
 * Custom setters and getters
 */

TEST_CASE(CustomSetter1)
{
	Property<int> pSize(277, [&](const int &_new)
	{
		pSize.raw() = std::max(std::min(100, _new), 0);
	});

	ASSERT_EQ(100, pSize);
}


TEST_CASE(CustomSetter2)
{
	Property<int> pSize(1501, [&](const int &_new)
	{
		pSize.raw() = std::max(std::min(850, _new), 0);
	});

	ASSERT_EQ(850, pSize);
	pSize = 430;
	ASSERT_EQ(430, pSize);
	pSize = 853;
	ASSERT_EQ(850, pSize);
}


TEST_CASE(CustomSetter_NoDefaultValue)
{
	Property<int> pSize([&](const int &_new)
	{
		pSize.raw() = std::max(std::min(5, _new), 0);
	});

	pSize = 49;
	ASSERT_EQ(5, pSize);
	pSize = 4;
	ASSERT_EQ(4, pSize);
}


TEST_CASE(CustomGetter1)
{
	Property<int> pSize(0, [&]() -> int
	{
		return pSize.raw() + 100;
	});

	pSize = 1234;
	ASSERT_EQ(1334, pSize);
}


TEST_CASE(CustomGetter2)
{
	struct Test
	{
		Property<int> pSize = Property<int>(0, [this]() -> int
		{
			return pSize.raw() + 100;
		});
	} t1;

	ASSERT_EQ(100, t1.pSize);
}


TEST_CASE(CustomSetterAndGetter)
{
	struct Test
	{
		Property<int> pAmount = Property<int>(14,
				[this](const int &_new) {
					pAmount.raw() = _new+5;
				},

				[this]() -> int {
					return pAmount.raw()+1;
				});
	} t1;

	ASSERT_EQ(20, t1.pAmount);
	ASSERT_EQ(19, t1.pAmount.raw());
}



/*
 * Custom setters and getters with simplified macros
 */

TEST_CASE(MacroCustomSetter)
{
	struct Test
	{
		Property_Set(int, pAmount, 80,
		{
			pAmount.raw() = _newValue-4;
		})
	} t1;

	ASSERT_EQ(76, t1.pAmount);
}


TEST_CASE(MacroCustomGetter)
{
	struct Test
	{
		Property_Get(int, pAmount, 80,
		{
			return pAmount.raw() + 6;
		})
	} t1;

	ASSERT_EQ(86, t1.pAmount);
}


TEST_CASE(MacroCustomSetterGetter)
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

	ASSERT_EQ(99.0, t1.pAmount);
	ASSERT_EQ(103.0, t1.pAmount.raw());
}


/*
 * Custom onChanged listeners
 */

TEST_CASE(CustomOnChangedListener)
{
	Property<bool> pEnabled = true;
	bool wasNotified1 = false;
	bool wasNotified2 = false;

	pEnabled.addOnChangedListener([&]{ wasNotified1 = true; });
	pEnabled.addOnChangedListener([&]{ wasNotified2 = true; });

	pEnabled = !pEnabled;

	ASSERT_TRUE(wasNotified1) << "On changed listener 1 was not notified";
	ASSERT_TRUE(wasNotified2) << "On changed listener 2 was not notified";
}


TEST_CASE(CustomTypeUsage)
{
	struct Test
	{
		int size;

		Test& operator +=(const Test &rhs)
		{
			size += rhs.size;
			return *this;
		}
	};

	Property<Test> p1;
	p1().size = 10;
}
