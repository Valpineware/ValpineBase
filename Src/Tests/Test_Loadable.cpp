#include <ValpineBase/Test/Test.h>
#include <ValpineBase/Loadable.h>

using namespace ValpineBase;
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
	Loadable loadable;

	auto f = []
	{
		Loadable::Begin b(&loadable);
	};

	Assert::False(loadable.pHasLoaded);
	f();
	Assert::True(loadable.pHasLoaded);


	//TODO need a way to know if an error occured even though
	//we have at least loaded already
}

