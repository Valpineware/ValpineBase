//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <ValpineBase/Test.h>
#include <ValpineBase/Loadable.h>

#include <QtCore/QDateTime>

using namespace vbase;

class Test_Loadable : public test::Class
{
	Q_OBJECT

private slots:
	VTEST void simpleCheck()
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


		Assert_False(loadable.pHasLoaded);
		f();
		Assert_True(loadable.pHasLoaded);
		Assert_True(didIsLoading);
	}


	VTEST void hey()
	{
		Verify_Eq(40, 50);
		Verify_Failure("Hello");
		Verify_False(true);
		Verify_True(false);
	}
};

ADD_TESTCLASS(Test_Loadable)
#include "Test_Loadable.moc"
