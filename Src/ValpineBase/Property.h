//==================================================================================================================|
// Created 2015.04.20 by Daniel L. Watkins
//
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _ValpineBase_Property_h
#define _ValpineBase_Property_h

#include <functional>
#include <utility>
#include <QtCore/QList>
#include <QtCore/QVariant>

namespace vbase
{
    /**
     *
     */
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
		 * @brief Property<T> Copy constructor. Only the value of the property
		 * is copied. Custom set/get functions and on change listeners are not
		 * copied into the new property.
		 * @param rhs
		 */
		Property<T>(const Property<T> &rhs) :
			mValue(rhs.mValue),
			mSetFunction(DEFAULT_SET),
			mGetFunction(DEFAULT_GET)
		{
		}

		/**
		 * @brief Property<T> Copy assignment operator. Only the value of the
		 * property is copied. Custom set/get functions and on change listeners
		 * are not copied into the new property.
		 * @param rhs
		 */
        Property<T> operator=(const Property<T> &rhs)
        {
            set(rhs.raw());
            return *this;
        }

		Property<T>(const T &value) :
			mValue(value),
			mSetFunction(DEFAULT_SET),
			mGetFunction(DEFAULT_GET)
		{
		}

		Property<T>(const T &value, const SetFunction &setFunction) :
			mValue(value),
			mSetFunction(setFunction),
			mGetFunction(DEFAULT_GET)
		{
		}

		Property<T>(const SetFunction &setFunction) :
			mSetFunction(setFunction),
			mGetFunction(DEFAULT_GET)
		{
		}

		Property<T>(const T &value, const GetFunction &getFunction) :
			mValue(value),
			mSetFunction(DEFAULT_SET),
			mGetFunction(getFunction)
		{
		}

		Property<T>(const T &value,
					const SetFunction &setFunction,
					const GetFunction &getFunction) :
			mValue(value),
			mSetFunction(setFunction),
			mGetFunction(getFunction)
		{
		}


		operator T() const
		{
			return const_cast<Property<T>*>(this)->mGetFunction();
		}


		T& operator()() { return mValue; }
		const T& operator()() const { return mValue; }
		operator QVariant() const { return mValue; }

		Property<T>& operator=(const T &rhs)
		{
			set(rhs);
			return *this;
		}


		T& raw() { return mValue; }	//TODO this really should call the get func
		const T& raw() const { return mValue; }


		void addOnChangedListener(ListenerFunction listenerFunction)
		{
			mOnChangedListeners.append(listenerFunction);
		}


		/////////////////////////////////////////
		// Extra operators

		template<typename U=T>
		Property<T> operator +=(const U &rhs)
		{
            set(mValue + rhs);
			return *this;
		}

        template<typename U=T>
        Property<T> operator -=(const U &rhs)
        {
            set(mValue - rhs);
            return *this;
        }

        template<typename U=T>
        Property<T> operator *=(const U &rhs)
        {
            set(mValue * rhs);
            return *this;
        }

        template<typename U=T>
        Property<T> operator /=(const U &rhs)
        {
            set(mValue / rhs);
            return *this;
        }

        template<typename U=T>
        Property<T> operator %=(const U &rhs)
        {
            set(mValue % rhs);
            return *this;
        }

		template<typename U=T>
		bool operator ==(const U &rhs) const
		{
			return mValue == rhs;
		}

		T operator -() const
		{
			return -mValue;
		}

	private:
		T mValue;
		QList<ListenerFunction> mOnChangedListeners;
		mutable SetFunction mSetFunction;
		mutable GetFunction mGetFunction;

		void set(const T &value)
		{
			mSetFunction(value);
			notifyOnChangedListeners();
		}


		void notifyOnChangedListeners()
		{
			for (ListenerFunction &lf : mOnChangedListeners)
				lf();
		}
	};


	template <typename T>
	T operator -(const T &lhs, const Property<T> &rhs)
	{
		return lhs-rhs.raw();
	}
}

#define Property_Set(type, name, defaultValue, setBody) \
            ::vbase::Property<type> name = ::vbase::Property<type>(defaultValue, \
					[this](const type &_newValue) setBody);

#define Property_Get(type, name, defaultValue, getBody) \
            ::vbase::Property<type> name = ::vbase::Property<type>(defaultValue, \
					[this]() -> type getBody);

#define Property_SetGet(type, name, defaultValue, setBody, getBody) \
            ::vbase::Property<type> name = ::vbase::Property<type>(defaultValue, \
				[this](const type &_newValue) setBody, \
				[this]() -> type getBody);

#undef DEFAULT_SET
#undef DEFAULT_GET

#endif
