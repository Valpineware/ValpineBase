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
#include <ValpineBase/Test/Result.h>
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
	Failure* makeDefaultFailure() const
	{
		auto r = new Failure;
		r->filepath = mFilepath;
		r->lineNumber = mLineNumber;

		return r;
	}

private:
	Class *mHostClass = nullptr;
	QString mFilepath;
	int mLineNumber = -1;
};

}}

#endif
