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
#include <ValpineBase/Test/Message.h>
#include <ValpineBase/Test/Class.h>
#include <ValpineBase/Test/Suite.h>

namespace vbase { namespace test {

class FailureBase
{
public:
	FailureBase(Class *hostClass, const QString &filepath, int lineNumber) :
		mHostClass(hostClass),
		mFilepath(filepath),
		mLineNumber(lineNumber)
	{
	}

protected:
	template<typename T, typename U>
	bool areEq(const QString &verbatimActual, const QString &verbatimExpected,
			   const T &actual, const U &expected)
	{
		if (actual != expected)
		{
			Message *failure = makeDefaultFailure();
			failure->details.append(verbatimActual + " != " + verbatimExpected);

			failure->details.append(QString("Expected: ") + UniversalToString::toString(expected));
			failure->details.append(QString("Actual: ") + UniversalToString::toString(actual));

			logFailure(failure);
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

			logFailure(failure);
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

			logFailure(failure);
			return false;
		}

		return true;
	}


	bool failure(const QString &message)
	{
		auto *failure = makeDefaultFailure();
		failure->details << message;

		logFailure(failure);
		return false;
	}


	Message* makeDefaultFailure() const
	{
		auto r = new Message;
		r->filepath = mFilepath;
		r->lineNumber = mLineNumber;
		r->type = Message::Type::Error;

		return r;
	}

private:
	Class *mHostClass = nullptr;
	QString mFilepath;
	int mLineNumber = -1;

	void logFailure(Message *failure)
	{
		mHostClass->hostSuite->post(mHostClass->metaObject()->className(),
									mHostClass->currentlyExecutingMethodName,
									failure);
	}
};

}}

#endif
