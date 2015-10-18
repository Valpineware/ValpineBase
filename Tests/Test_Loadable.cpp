//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <ValpineBase/Test.h>
#include <ValpineBase/Loadable.h>

#include <QtCore/QDateTime>
#include <QtCore/QThread>

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


AssertFalsese(loadable.pHasLoaded);
		f();
		AssertTrue(loadable.pHasLoaded);
		AssertTrue(didIsLoading);

	}
};

ADD_TESTCLASS(Test_Loadable)
#include "Test_Loadable.moc"
