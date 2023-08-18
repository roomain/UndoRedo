#pragma once
#include <memory>

class IRecordObject;

template<typename T>
class MShared_ptr : public std::shared_ptr<T>
{
public:
    using std::shared_ptr<T>::shared_ptr;
    MShared_ptr(const std::shared_ptr<T>& a_other) : std::shared_ptr<T>(a_other) {};
    MShared_ptr(std::shared_ptr<T>&& a_other) : std::shared_ptr<T>(a_other) {};
    ~MShared_ptr()
    {
        if constexpr (std::is_base_of_v<IRecordObject, T>)
        {
            if (use_count() == 1)
                operator->()assertDelete();
        }
    }
};