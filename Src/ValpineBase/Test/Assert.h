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

namespace vbase { namespace test {

class Assert
{
public:
	Assert() = delete;

	Assert(Class *hostClass, const QString &filepath, int lineNumber) :
		mHostClass(hostClass),
		mFilepath(filepath),
		mLineNumber(lineNumber)
	{

	}


	template<typename T, typename U>
	void areEq(const QString &verbatimActual, const QString &verbatimExpected,
			   const T &actual, const U &expected) const
	{
		if (actual != expected)
		{
			TestAssertException tfe;
			tfe.failure = makeDefaultFailure();
			tfe.failure->message.append(verbatimActual + " != " + verbatimExpected);

			tfe.failure->message.append(QString("Expected: ") + UniversalToString::toString(expected));
			tfe.failure->message.append(QString("Actual: ") + UniversalToString::toString(actual));

			throw tfe;
		}
	}


	void isTrue(const QString &verbatim, bool what) const
	{
		if (!what)
		{
			TestAssertException tfe;
			tfe.failure = makeDefaultFailure();
			tfe.failure->message.append(QString("Expected ") + verbatim + " to be true. Got false.");

			throw tfe;
		}
	}


	void isFalse(const QString &verbatim, bool what) const
	{
		if (what)
		{
			TestAssertException tfe;
			tfe.failure = makeDefaultFailure();
			tfe.failure->message.append(QString("Expected ") + verbatim + " to be false. Got true.");

			throw tfe;
		}
	}


	void failure(const QString &message) const
	{
		TestAssertException tfe;
		tfe.failure = makeDefaultFailure();
		tfe.failure->message << message;

		throw tfe;
	}

private:
	Class *mHostClass = nullptr;
	QString mFilepath;
	int mLineNumber = -1;

	Failure* makeDefaultFailure() const
	{
		auto r = new Failure;
		r->filepath = mFilepath;
		r->lineNumber = mLineNumber;

		return r;
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
