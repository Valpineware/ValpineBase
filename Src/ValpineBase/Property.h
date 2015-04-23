//==================================================================================================================|
// Created 2015.04.20 by Daniel L. Watkins
//
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _ValpineBase_Property_h
#define _ValpineBase_Property_h

#include <functional>

namespace ValpineBase
{
	template <typename T>
	class Property
	{
	public:
		using SetFunction = std::function<void(const T&, T&)>;
		using GetFunction = std::function<T(T&)>;	//TODO return copy or cosnt ref?
													//maybe we can template-specialize primitives to return by value

#define DEFAULT_SET [](const T &_new, T &_value) { _value = _new; }
#define DEFAULT_GET [](const T &_value) -> T { return _value; }

	public:
		/**
		 * Default constructor
		 */
		Property<T>() :
			mSetFunction(DEFAULT_SET),
			mGetFunction(DEFAULT_GET)
		{
		}

		/**
		 * @param value Default property value.
		 */
		Property<T>(const T &value) :
			Property<T>()
		{
			*this = value;
		}

		/**
		 * @brief Constructor
		 * @param value Default property value.
		 * @param setFunction Custom set function.
		 */
		Property<T>(const T &value, const SetFunction &setFunction) :
			mSetFunction(setFunction),
			mGetFunction(DEFAULT_GET)
		{
			*this = value;
		}

		/**
		 * @brief Constructor
		 * @param setFunction Custom set function
		 */
		Property<T>(const SetFunction &setFunction) :
			mSetFunction(setFunction),
			mGetFunction(DEFAULT_GET)
		{
		}

		/**
		 * @brief Constructor
		 * @param value Default property value
		 * @param getFunction Custom get function.
		 */
		Property<T>(const T &value, const GetFunction &getFunction) :
			mSetFunction(DEFAULT_SET),
			mGetFunction(getFunction)
		{
			*this = value;
		}

		operator T() const
		{
			return mValue;
		}


		Property<T>& operator=(const T &rhs)
		{
			mSetFunction(rhs, mValue);
			return *this;
		}

	private:
		T mValue;
		mutable SetFunction mSetFunction;
		mutable GetFunction mGetFunction;
	};
}

#undef DEFAULT_SET
#undef DEFAULT_GET

#endif
