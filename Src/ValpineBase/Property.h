//==================================================================================================================|
// Created 2015.04.20 by Daniel L. Watkins
//
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _ValpineBase_Property_h
#define _ValpineBase_Property_h

#include <functional>
#include <QtCore/QList>

namespace ValpineBase
{
	template <typename T>
	class Property
	{
	public:
		using SetFunction = std::function<void(const T&)>;
		using GetFunction = std::function<T(void)>;	//TODO return copy or cosnt ref?
													//maybe we can template-specialize primitives to return by value
		using ListenerFunction = std::function<void(void)>;

#define DEFAULT_SET [this](const T &_new) { mValue = _new; }
#define DEFAULT_GET [this]() -> T { return mValue; }

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


		/**
		 * @brief Constructor
		 * @param value Default property value
		 * @param setFunction Custom set function
		 * @param getFunction Custom get function
		 */
		Property<T>(const T &value,
					const SetFunction &setFunction,
					const GetFunction &getFunction) :
			mSetFunction(setFunction),
			mGetFunction(getFunction)
		{
			*this = value;
		}

		operator T() const
		{
			return const_cast<Property<T>*>(this)->mGetFunction();
		}


		Property<T>& operator=(const T &rhs)
		{
			mSetFunction(rhs);
			notifyOnChangedListeners();

			return *this;
		}


		T& raw() { return mValue; }


		void addOnChangedListener(ListenerFunction listenerFunction)
		{
			mOnChangedListeners.append(listenerFunction);
		}

	private:
		T mValue;
		QList<ListenerFunction> mOnChangedListeners;
		mutable SetFunction mSetFunction;
		mutable GetFunction mGetFunction;

		void notifyOnChangedListeners()
		{
			for (ListenerFunction &lf : mOnChangedListeners)
				lf();
		}
	};
}

#define Property_Set(type, name, defaultValue, setBody) \
			Property<type> name = Property<type>(defaultValue, \
					[this](const type &_newValue) setBody);

#define Property_Get(type, name, defaultValue, getBody) \
			Property<type> name = Property<type>(defaultValue, \
					[this]() -> type getBody);

#define Property_SetGet(type, name, defaultValue, setBody, getBody) \
			Property<type> name = Property<type>(defaultValue, \
				[this](const type &_newValue) setBody, \
				[this]() -> type getBody);

#undef DEFAULT_SET
#undef DEFAULT_GET

#endif
