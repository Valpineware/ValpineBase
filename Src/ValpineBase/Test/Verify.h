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
			   const T &actual, const U &expected) const
	{
		if (actual != expected)
		{
			auto resultFailure = makeDefaultFailure();
			resultFailure->message.append(verbatimActual + " != " + verbatimExpected);

			resultFailure->message.append(QString("Expected: ") + UniversalToString::toString(expected));
			resultFailure->message.append(QString("Actual: ") + UniversalToString::toString(actual));

			qWarning() << "areEq logged";
			//FIXME log the failure
		}
	}


	void isTrue(const QString &verbatim, bool what) const
	{
		if (!what)
		{
			auto resultFailure = makeDefaultFailure();
			resultFailure->message.append(QString("Expected ") + verbatim + " to be true. Got false.");

			qWarning() << "isTrue logged";
			//FIXME log the failure
		}
	}


	void isFalse(const QString &verbatim, bool what) const
	{
		if (what)
		{
			auto resultFailure = makeDefaultFailure();
			resultFailure->message.append(QString("Expected ") + verbatim + " to be false. Got true.");

			qWarning() << "isFalse logged";
			//FIXME log the failure
		}
	}


	void failure(const QString &message) const
	{
		auto resultFailure = makeDefaultFailure();
		resultFailure->message << message;

		qWarning() << "failure logged";
		//FIXME log the failure
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
