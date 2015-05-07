#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Loadable.h>

using namespace vbase;

#define CLASS Test_Loadable
TEST_CLASS
{
protected:
};

/*
 * General construction and assignment
 */

TEST_CASE(SimpleSetCheck1)
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


	ASSERT_FALSE(loadable.pHasLoaded);
	f();
	ASSERT_TRUE(loadable.pHasLoaded);
	ASSERT_TRUE(didIsLoading);
}

