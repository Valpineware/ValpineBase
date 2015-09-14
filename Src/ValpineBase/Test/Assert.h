//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Assert_h
#define vbase_test_Assert_h

#include <sstream>
#include <type_traits>
#include <QString>

#include <ValpineBase/Test/Result.h>
#include <ValpineBase/Test/Class.h>
#include <ValpineBase/Test/Suite.h>

namespace vbase { namespace test
{
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


        template<typename T>
        void areEq(const QString &verbatimActual, const QString &verbatimExpected,
                   const T &actual, const T &expected) const
        {
            if (actual != expected)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();
                tfe.pResultFailure->pMessage().append(verbatimActual + " != " + verbatimExpected);

		tfe.pResultFailure->pMessage().append(QString("Expected: ") + Assert::formatRaw(expected));
		tfe.pResultFailure->pMessage().append(QString("Actual: ") + Assert::formatRaw(actual));

               throw tfe;
            }
        }


        void isTrue(const QString &verbatim, bool what) const
        {
            if (!what)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();
                tfe.pResultFailure->pMessage().append(QString("Expected ") + verbatim + " to be true. Got false.");

                throw tfe;
            }
        }


        void isFalse(const QString &verbatim, bool what) const
        {
            if (what)
            {
                TestFailureException tfe;
                tfe.pResultFailure = makeDefaultResultFailure();
                tfe.pResultFailure->pMessage().append(QString("Expected ") + verbatim + " to be false. Got true.");

                throw tfe;
            }
        }


        void failure(const QString &message) const
        {
            TestFailureException tfe;
            tfe.pResultFailure = makeDefaultResultFailure();
            tfe.pResultFailure->pMessage() << message;

            throw tfe;
        }

    private:
        Class *mHostClass = nullptr;
        QString mFilepath;
        int mLineNumber = -1;

        ResultFailure* makeDefaultResultFailure() const
        {
            auto r = new ResultFailure;
            r->pFilepath = mFilepath;
            r->pLineNumber = mLineNumber;
            r->pExecutionTime = mHostClass->pExecutionTimer().elapsed();

            return r;
        }


    private:
	template<typename, typename T>
	struct has_toString {
	    static_assert(
		std::integral_constant<T, false>::value,
		"Second template parameter needs to be of function type.");
	};

	template<typename C, typename Ret, typename... Args>
	struct has_toString<C, Ret(Args...)> {
	private:
	    template<typename T>
	    static constexpr auto check(T*)
	    -> typename
		std::is_same<
		    decltype( std::declval<T>().toString( std::declval<Args>()... ) ),
		    Ret>::type;

	    template<typename>
	    static constexpr std::false_type check(...);

	    typedef decltype(check<C>(0)) type;

	public:
	    static constexpr bool value = type::value;
	};


	template<typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
	static QString formatRaw(const T &what)
	{
	    return QString::number(what);
	}


	template<typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static QString formatRaw(const T &what)
	{
	    return QString::number(static_cast<long double>(what), 'g', 10);
	}


	static QString formatRaw(const QString &what)
	{
	    return static_cast<QString>(what);
	}


	static QString formatRaw(const std::string &what)
	{
	    return QString::fromStdString(what);
	}


	static QString formatRaw(const std::wstring &what)
	{
	    return QString::fromStdWString(what);
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
