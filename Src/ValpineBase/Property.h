//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Property_h
#define _ValpineBase_Property_h

#include <functional>
#include <utility>
#include <QtCore/QList>
#include <QtCore/QVariant>

namespace vbase
{
    /**
     * \brief A wrapper that behaves like the template type without the need for
     * declaring boilerplate setters, getters, and onChanged signals.
     *
     * A Property<T> should be declared as a public instance variable to replace
     * the need for a setter, getter, and/or onChanged signal. A Property<T> has
     * the following behavior:
     *      - Implicitly converts to type T
     *      - Assigns from another:
     *          -# T
     *          -# Property<T>
     *          -# Implicitly convertible Property<U> (TODO really?)
     *      - Forwards operators to the underlying T
     *
     * By default, a Property<T> declared as a public instance variable may be
     * treated as such. It will behave semantically the same as if it was simply
     * declared as a T. There is, however, a slight performance penalty with a
     * Property<T> because every read and write operation requires an additional
     * call to a custom read/write implementation. A simple, default Property
     * may be declared as in the following example:
     *
     * \code{.cpp}
     * class A
     * {
     * public:
     *      Property<int> pAmount = 100;
     * };
     *
     * int main()
     * {
     *      A a1;
     *      a1.pAmount += 15;
     *      qDebug() << a1.pAmount;     //output is "115"
     * }
     * \endcode
     *
     * Custom setters and getters may be supplied with ease using the macros
     * Property_Set, Property_Get, or Property_SetGet. Note that initializing
     * a property with a value does not run through the setter. For example:
     *
     * \code{.cpp}
     * class A
     * {
     * public:
     *      Property_SetGet(int, pAmount, 75,
     *
     *      //custom setter with _newValue as an argument defined in the real
     *      //lamda under the hood. Only assign to raw() to avoid stackoverflow.
     *      {
     *          qDebug() << "Setting";
     *          pAmount.raw() = _newValue;
     *      },
     *      //custom getter. Only access from raw() to avoid stackoverflow.
     *      {
     *          qDebug() << "Getting";
     *          return pAmount.raw();
     *      })
     * };
     *
     * int main()
     * {
     *      A a1;
     *      a1 = 50;        //prints "Setting"
     *      int b = a1;     //prints "Getting"
     * }
     * \endcode
     */
	template <typename T>
	class Property
	{
	public:
		using SetFunction = std::function<void(const T&)>;
                using GetFunction = std::function<T&(void)>;
                // ^ TODO return copy or reference? it would be nice for end user to customize this if needed

		using ListenerFunction = std::function<void(void)>;

#define DEFAULT_SET [this](const T &_new) { mValue = _new; }
#define DEFAULT_GET [this]() -> T& { return mValue; }

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

		Property<T>(const T &value,
					const SetFunction &setFunction,
					const GetFunction &getFunction) :
			mValue(value),
			mSetFunction(setFunction),
			mGetFunction(getFunction)
		{
		}


                //TODO we really need a test for all type of property access

		operator T() const
		{
                    return const_cast<Property<T>*>(this)->mGetFunction();
		}


                T& operator()() { return const_cast<T&>(mGetFunction()); }
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
            ::vbase::Property<type> name { ::vbase::Property<type>(defaultValue, \
                    [this](const type &_newValue) setBody) };

#define Property_Get(type, name, defaultValue, getBody) \
            ::vbase::Property<type> name = ::vbase::Property<type>(defaultValue, \
                                        [this]() -> type& getBody);

#define Property_SetGet(type, name, defaultValue, setBody, getBody) \
            ::vbase::Property<type> name = ::vbase::Property<type>(defaultValue, \
				[this](const type &_newValue) setBody, \
                                [this]() -> type& getBody);

#undef DEFAULT_SET
#undef DEFAULT_GET

#endif
