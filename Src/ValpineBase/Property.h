#include <functional>

template <typename T>
class Property
{
public:
	using SetFunction = std::function<void(const T&, T&)>;

public:
	Property<T>() :
		mSetFunction([](const T &_new, T &_value) { _value = _new; })
	{
	}

	Property<T>(const T &value) :
		Property<T>()
    {
		mValue = value;
    }

	Property<T>(const T &value, const SetFunction &setFunction) :
		mSetFunction(setFunction)
	{
		*this = value;
	}


	Property<T>(const SetFunction &setFunction) :
		mSetFunction(setFunction)
	{
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
	SetFunction mSetFunction;
};
