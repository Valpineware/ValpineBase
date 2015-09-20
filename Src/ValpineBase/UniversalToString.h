//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_UniversalToString_H
#define _ValpineBase_UniversalToString_H

namespace vbase {

class UniversalToString
{
public:
	template<typename T>
	static QString toString(const T &what)
	{
		return QString("\"") + _formatRaw(what) + "\"";
	}

private:
	template<typename, typename T>
	struct has_toString
	{
		static_assert(
				std::integral_constant<T, false>::value,
				"Second template parameter needs to be of function type.");
	};

	template<typename C, typename Ret, typename... Args>
	struct has_toString<C, Ret(Args...)>
	{
		private:
		template<typename T>
		static constexpr auto check(T*) -> typename
				std::is_same<
				decltype(std::declval<T>().toString( std::declval<Args>()... )),
				Ret>::type;

		template<typename>
		static constexpr std::false_type check(...);

		typedef decltype(check<C>(0)) type;

		public:
		static constexpr bool value = type::value;
	};


	template<typename T, typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
	static QString _formatRaw(const T &what)
	{
		return QString::number(static_cast<int>(what));
	}


	template<typename T,
			 typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
	static QString _formatRaw(T what)
	{
		return QString::number(what);
	}


	template<typename T,
			 typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
	static QString _formatRaw(T what)
	{
		return QString::number(static_cast<long double>(what), 'g', 10);
	}


	static QString _formatRaw(bool what)
	{
		return what ? "true" : "false";
	}


	template<typename T,
			 typename std::enable_if<
				 has_toString<T,QString(void)>::value>::type* = nullptr>
	static QString _formatRaw(const T &what)
	{
		return what.toString();
	}


	static QString _formatRaw(const QString &what)
	{
		return static_cast<QString>(what);
	}


	static QString _formatRaw(const char* what)
	{
		return QString(what);
	}


	static QString _formatRaw(const std::string &what)
	{
		return QString::fromStdString(what);
	}


	static QString _formatRaw(const std::wstring &what)
	{
		return QString::fromStdWString(what);
	}
};

}

#endif
