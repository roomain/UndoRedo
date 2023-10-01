#pragma once
/***********************************************
* @headerfile TScoped.h
* @date 01 / 08 / 2023
* @author Roomain
************************************************/

template<typename Type>
class TScoped
{
private:
    Type& m_original;
    Type m_saved;

public:
    TScoped() = delete;
    TScoped(const TScoped<Type>&) = delete;
    TScoped(TScoped<Type>&& a_other) : m_original{ a_other.m_original },
        m_saved{ std::move(a_other.m_saved) } {}
    TScoped& operator = (const TScoped<Type>& a_other) = delete;
    TScoped& operator = (TScoped<Type>&& a_other)
    {
        m_original = a_other.m_original;
        m_saved = std::move(a_other.m_saved);
    }

    explicit TScoped(Type& a_original) : m_original{ a_original }, m_saved{ a_original }
    {

    }

    ~TScoped()
    {
        std::swap(m_saved, m_original);
    }

    Type& operator = (const Type& a_other)
    {
        m_original = a_other;
        return m_original;
    }

    operator Type()
    {
        return m_original;
    }
};
