#pragma once
#include <memory>
#include "RecordAssertions.h"

template<typename T>
class MShared_ptr : public std::shared_ptr<T>
{
public:
    using std::shared_ptr<T>::shared_ptr;
    MShared_ptr(const std::shared_ptr<T>& a_other) : std::shared_ptr<T>(a_other) {};
    MShared_ptr(std::shared_ptr<T>&& a_other) : std::shared_ptr<T>(a_other) {};
    MShared_ptr(const std::weak_ptr<T>& a_other) : std::shared_ptr<T>(a_other) {};
    MShared_ptr(std::weak_ptr<T>&& a_other) : std::shared_ptr<T>(a_other) {};
    template<typename U>
    MShared_ptr(std::shared_ptr<T>&& a_other, T* a_ptr) noexcept : std::shared_ptr<T>(a_other, a_ptr) {};
    virtual ~MShared_ptr()
    {
        if constexpr (std::is_base_of_v<IRecordObject, T>)
        {
            if (std::shared_ptr<T>::use_count() == 1 && std::shared_ptr<T>::get())
            {
                assertDeletion(std::shared_ptr<T>::get());
            }
        }
    }
};