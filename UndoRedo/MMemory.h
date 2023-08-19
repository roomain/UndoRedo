#pragma once
#include "MShared_ptr.h"

template<typename T>
class MShared_from_this
{
    template<typename U, typename ...Args>
    friend std::shared_ptr<U> make_MShared(Args&& ...);

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

    MShared_ptr<const T> shared_from_this()const
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

    std::weak_ptr<const T> weak_from_this()const
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
    if constexpr (std::is_base_of_v<MShared_from_this<T>, T>)
        pObj->m_Wptr = pObj;
    return pObj;
}