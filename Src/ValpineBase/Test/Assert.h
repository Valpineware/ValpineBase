//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Assert_h
#define vbase_test_Assert_h

#include <sstream>
#include <type_traits>
#include <QString>

#include <ValpineBase/UniversalToString.h>
#include <ValpineBase/Test/Result.h>
#include <ValpineBase/Test/Class.h>
#include <ValpineBase/Test/Suite.h>
#include <ValpineBase/Test/FailureBase.h>

namespace vbase { namespace test {

class Assert : public FailureBase
{
public:
	Assert() = delete;

	Assert(Class *hostClass, const QString &filepath, int lineNumber) :
		FailureBase(hostClass, filepath, lineNumber)
	{

	}


	template<typename T, typename U>
	void areEq(const QString &verbatimActual, const QString &verbatimExpected,
			   const T &actual, const U &expected)
	{
		if (!FailureBase::areEq(verbatimActual, verbatimExpected,
								actual, expected))
			throw TestAssertException();
	}


	void isTrue(const QString &verbatim, bool what)
	{
		if (!FailureBase::isTrue(verbatim, what))
			throw TestAssertException();
	}


	void isFalse(const QString &verbatim, bool what)
	{
		if (!FailureBase::isFalse(verbatim, what))
			throw TestAssertException();
	}


	void failure(const QString &message)
	{
		if (!FailureBase::failure(message))
			throw TestAssertException();
	}
};

}}

#define Assert_Eq(actual, expected) \
	::vbase::test::Assert(this, QString(__FILE__), __LINE__).areEq( \
	QString(#actual), QString(#expected), \
	actual, expected)

#define Assert_True(what) \
	::vbase::test::Assert(this, QString(__FILE__), __LINE__).isTrue( \
	QString(#what), what)

#define Assert_False(what) \
	::vbase::test::Assert(this, QString(__FILE__), __LINE__).isFalse( \
	QString(#what), what)

#define Assert_Failure(message) \
	::vbase::test::Assert(this, QString(__FILE__), __LINE__).failure(message)

#endif
