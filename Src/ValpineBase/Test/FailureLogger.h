//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_FailureBase_h
#define vbase_test_FailureBase_h

#include <sstream>
#include <type_traits>

#include <QtCore/QString>

#include <ValpineBase/UniversalToString.h>
#include <ValpineBase/Test/Failure.h>
#include <ValpineBase/Test/Class.h>
#include <ValpineBase/Test/Suite.h>

namespace vbase { namespace test {

class FailureLogger
{
public:
	FailureLogger(Class *hostClass, const QString &filepath, int lineNumber) :
		mHostClass(hostClass),
		mFilepath(filepath),
		mLineNumber(lineNumber)
	{
	}

public:
	template<typename T, typename U>
	bool areEq(const QString &verbatimActual, const QString &verbatimExpected,
			   const T &actual, const U &expected)
	{
		if (actual != expected)
		{
			Failure *failure = makeDefaultFailure();
			failure->details.append(verbatimActual + " != " + verbatimExpected);

			failure->details.append(QString("Expected: ") + UniversalToString::toString(expected));
			failure->details.append(QString("Actual: ") + UniversalToString::toString(actual));

			postFailure(failure);
			return false;
		}

		return true;
	}


	bool isTrue(const QString &verbatim, bool what)
	{
		if (!what)
		{
			auto *failure = makeDefaultFailure();
			failure->details.append(QString("Expected ") + verbatim + " to be true. Got false.");

			postFailure(failure);
			return false;
		}

		return true;
	}


	bool isFalse(const QString &verbatim, bool what)
	{
		if (what)
		{
			auto *failure = makeDefaultFailure();
			failure->details.append(QString("Expected ") + verbatim + " to be false. Got true.");

			postFailure(failure);
			return false;
		}

		return true;
	}


	bool failure(const QString &message)
	{
		auto *failure = makeDefaultFailure();
		failure->details << message;

		postFailure(failure);
		return false;
	}


	bool warning(const QString &message)
	{
		auto *failure = makeDefaultFailure();
		failure->details << message;
		failure->type = Failure::Type::Warn;

		postFailure(failure);
		return false;
	}


	Failure* makeDefaultFailure() const
	{
		auto r = new Failure;
		r->filepath = mFilepath;
		r->lineNumber = mLineNumber;
		r->type = Failure::Type::Error;

		return r;
	}

private:
	Class *mHostClass = nullptr;
	QString mFilepath;
	int mLineNumber = -1;

	void postFailure(Failure *failure)
	{
		mHostClass->hostSuite->postFailure(mHostClass->metaObject()->className(),
									mHostClass->currentlyExecutingMethodName,
									failure);
	}
};

}}

#define Verify_Eq(actual, expected) \
	::vbase::test::FailureLogger(this, QString(__FILE__), __LINE__).areEq( \
	QString(#actual), QString(#expected), \
	actual, expected)

#define Verify_True(what) \
	::vbase::test::FailureLogger(this, QString(__FILE__), __LINE__).isTrue( \
	QString(#what), what)

#define Verify_False(what) \
	::vbase::test::FailureLogger(this, QString(__FILE__), __LINE__).isFalse( \
	QString(#what), what)

#define Post_Failure(message) \
	::vbase::test::FailureLogger(this, QString(__FILE__), __LINE__).failure(message)

#define Post_Warning(message) \
	::vbase::test::FailureLogger(this, QString(__FILE__), __LINE__).warning(message)


#endif
