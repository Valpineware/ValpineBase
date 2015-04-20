template <typename T>
class Property
{
public:
    Property<T>() = default;

    Property<T>(const T &rhs)
    {
        mValue = rhs;
    }

    operator T() const
    {
        return mValue;
    }

private:
    T mValue;
};
