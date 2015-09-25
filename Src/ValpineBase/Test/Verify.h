//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Verify_h
#define vbase_test_Verify_h

#include <ValpineBase/Test/FailureBase.h>

namespace vbase { namespace test {

class Verify : public FailureBase
{
public:
	Verify() = delete;

	Verify(Class *hostClass, const QString &filepath, int lineNumber) :
		FailureBase(hostClass, filepath, lineNumber)
	{
	}


	template<typename T, typename U>
	void areEq(const QString &verbatimActual, const QString &verbatimExpected,
			   const T &actual, const U &expected)
	{
		FailureBase::areEq(verbatimActual, verbatimExpected,
						   actual, expected);
	}


	void isTrue(const QString &verbatim, bool what)
	{
		FailureBase::isTrue(verbatim, what);
	}


	void isFalse(const QString &verbatim, bool what)
	{
		FailureBase::isFalse(verbatim, what);
	}


	void failure(const QString &message)
	{
		FailureBase::failure(message);
	}
};

}}

#define Verify_Eq(actual, expected) \
	::vbase::test::Verify(this, QString(__FILE__), __LINE__).areEq( \
	QString(#actual), QString(#expected), \
	actual, expected)

#define Verify_True(what) \
	::vbase::test::Verify(this, QString(__FILE__), __LINE__).isTrue( \
	QString(#what), what)

#define Verify_False(what) \
	::vbase::test::Verify(this, QString(__FILE__), __LINE__).isFalse( \
	QString(#what), what)

#define Verify_Failure(message) \
	::vbase::test::Verify(this, QString(__FILE__), __LINE__).failure(message)

#endif
