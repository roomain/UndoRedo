#pragma once
#include "MShared_ptr.h"
#include <type_traits>

template<typename T>
class MShared_from_this
{
    template<typename U, typename ...Args>
    friend MShared_ptr<U> make_MShared(Args&& ...);

private:
    std::weak_ptr<T> m_Wptr; /*!< weak pointer on this*/
	
public:
    constexpr MShared_from_this() : m_Wptr{} {}
    MShared_from_this(MShared_from_this<T>& a_other) : m_Wptr(a_other.m_Wptr) {}
    MShared_from_this(MShared_from_this<T>&& a_other)noexcept : m_Wptr(a_other.m_Wptr) {}
    virtual ~MShared_from_this() = default;
    MShared_ptr<T> shared_from_this()
    {
        if (!m_Wptr.lock())
            throw std::bad_weak_ptr{};
        return MShared_ptr<T>(m_Wptr);
    }

    MShared_ptr<const T> shared_from_const_this()const
    {
        if (!m_Wptr.lock())
            throw std::bad_weak_ptr{};
        return MShared_ptr<const T>(m_Wptr);
    }

    std::weak_ptr<T> weak_from_this()
    {
        if (!m_Wptr.lock())
            throw std::bad_weak_ptr{};
        return m_Wptr;
    }

    std::weak_ptr<const T> weak_from_const_this()
    {
        if (!m_Wptr.lock())
            throw std::bad_weak_ptr{};
        return m_Wptr;
    }
};

template<typename T, typename ...Args>
MShared_ptr<T> make_MShared(Args&& ...arg)
{
    MShared_ptr<T> pObj = std::make_shared<T>(arg...);    
    if constexpr (std::is_member_function_pointer_v<decltype(&T::weak_from_this)>)
        pObj->m_Wptr = pObj;
    return pObj;
}

template<typename T, typename U>
MShared_ptr<T> MStatic_pointer_cast(MShared_ptr<U>&& a_other)noexcept
{
    const auto ptr = static_cast<T*>(a_other.get());
    return MShared_ptr<T>(std::move(a_other), ptr);
}

template<typename T, typename U>
MShared_ptr<T> MStatic_pointer_cast(const MShared_ptr<U>& a_other)noexcept
{
    const auto ptr = static_cast<T*>(a_other.get());
    return MShared_ptr<T>(a_other, ptr);
}