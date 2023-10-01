#pragma once

#include <type_traits>

class RefObject;

class IRef
{
    friend class RefObject;

protected:
    mutable IRef* m_prevRef = nullptr;
    mutable IRef* m_nextRef = nullptr;

    virtual void reset() = 0;

public:
    void selfRemove()
    {
        if (m_prevRef)
        {
            m_prevRef->m_nextRef = m_nextRef;
            if (m_nextRef)
                m_nextRef->m_prevRef = m_prevRef;
        }
    }

    virtual void updateRefPointer(RefObject* const) = 0;

    virtual void updateRef(RefObject* const a_pRef)
    {
        updateRefPointer(a_pRef);
        m_nextRef->updateRef(a_pRef);
    }

    void appendRef(IRef* const a_ref)const
    {
        IRef* pCurRef = const_cast<IRef*>(this);
        IRef* pLastRef = m_nextRef;
        while (pLastRef)
        {
            pCurRef = pLastRef;
            pLastRef = pLastRef->m_nextRef;
        }
        pCurRef->m_nextRef = a_ref;
        a_ref->m_prevRef = pCurRef;
    }

    void unref()
    {
        reset();
        IRef* pLastRef = m_nextRef;
        while (pLastRef)
        {
            pLastRef->reset();
            pLastRef = pLastRef->m_nextRef;
        }
    }

    IRef* const next()const
    {
        return m_nextRef;
    }

    void setNext(IRef* const a_ref)
    {
        m_nextRef = a_ref;
    }

    IRef() = default;
    virtual ~IRef() = default;

};

template<typename Type> requires std::is_base_of_v<RefObject, Type>
class Ref : public IRef
{
    template<typename T>
    friend Ref<T> make_ref(const T&);

protected:
    Type* m_RefObject = nullptr;

    void updateRefPointer(RefObject* const a_pRef)
    {
        m_RefObject = static_cast<Type*>(a_pRef);
    }

    void reset()
    {
        m_RefObject = nullptr;
    }


    Ref(const Type& a_RefObject) : m_RefObject{ const_cast<Type*>(&a_RefObject) }
    {
        if (m_RefObject->m_RefList == nullptr)
        {
            m_RefObject->m_RefList = this;
        }
        else
        {
            m_RefObject->m_RefList->appendRef(this);
        }
    }

    Ref(Type* const a_RefObject) : m_RefObject{ a_RefObject }
    {
        if (m_RefObject)
        {
            if (m_RefObject->m_RefList == nullptr)
            {
                m_RefObject->m_RefList = this;
            }
            else
            {
                m_RefObject->m_RefList->appendRef(this);
            }
        }
    }

public:
    Ref() = default;

    Ref(const Ref<Type>& a_other) : m_RefObject{ a_other.m_RefObject }
    {
        if (static_cast<RefObject*>(m_RefObject)->m_RefList == nullptr)
        {
            static_cast<RefObject*>(m_RefObject)->m_RefList = this;
        }
        else
        {
            a_other.appendRef(this);
        }
    }

    ~Ref()
    {
        if (m_RefObject)
        {
            if (static_cast<RefObject*>(m_RefObject)->m_RefList == this)
            {
                static_cast<RefObject*>(m_RefObject)->m_RefList = m_nextRef;
            }
            else
            {
                selfRemove();
            }
        }
    }

    Type* const operator -> ()
    {
        return static_cast<Type*>(m_RefObject);
    }

    Type* const pointer() const
    {
        return static_cast<Type* const>(m_RefObject);
    }

    Ref<Type>& operator = (const Ref<Type>& a_other)
    {
        if (m_RefObject)
        {
            if (m_RefObject->m_RefList == this)
            {
                m_RefObject->m_RefList = m_nextRef;
            }
            else
            {
                m_prevRef->setNext(m_nextRef);
            }
        }

        m_RefObject = a_other.m_RefObject;
        if (m_RefObject->m_RefList == nullptr)
        {
            m_RefObject->m_RefList = this;
        }
        else
        {
            a_other.appendRef(this);
        }
        return *this;
    }

    size_t use_count()const
    {
        size_t counter = 0;
        if (m_RefObject)
        {
            IRef* pLastRef = m_RefObject->m_RefList;
            while (pLastRef)
            {
                counter++;
                pLastRef = pLastRef->next();
            }
        }
        return counter;
    }

    bool valid()const
    {
        return m_RefObject != nullptr;
    }

};


class RefObject
{
    //friend  IRef;

    template<typename Type> requires std::is_base_of_v<RefObject, Type>
    friend class Ref;

private:
    mutable IRef* m_RefList = nullptr;

public:
    RefObject()
    {
        //
    }

    RefObject(RefObject&& a_other)noexcept
    {
        m_RefList = a_other.m_RefList;
        if (m_RefList)
            m_RefList->updateRef(this);
    }

    ~RefObject()
    {
        if (m_RefList)
            m_RefList->unref();
    }

    void operator = (RefObject& a_other)
    {
        // nothing to do but function must be declared to enable std::swap
    }

    void operator = (RefObject&& a_other)noexcept
    {
        m_RefList = a_other.m_RefList;
        if (m_RefList)
            m_RefList->updateRef(this);
    }
};



template<typename T> //requires std::is_base_of_v<RefObject, T>
Ref<T> make_ref(const T& a_object)
{
    return Ref<T>(a_object);
}