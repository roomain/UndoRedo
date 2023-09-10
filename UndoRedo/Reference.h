#pragma once

template<typename Type>
class Reference;

template<typename T>
class Referenced
{
    friend  Reference<T>;

    template<typename Type>
    friend Reference<Type> make_ref(const Referenced<Type>&);
private:
    mutable Reference<T>* m_referenceList = nullptr;
public:
    Referenced()
    {
        //
    }

    ~Referenced()
    {
        if (m_referenceList)
            m_referenceList->unref();
    }

    Referenced<T>* const getRefPointer() const
    {
        return const_cast<Referenced<T>*const>(this);
    }

};



template<typename Type>
class Reference
{
    friend Referenced<Type>;

    template<typename T>
    friend Reference<T> make_ref(const T&);

private:
    mutable Reference<Type>* m_prevRef = nullptr;
    mutable Reference<Type>* m_nextRef = nullptr;
    Referenced<Type>* m_referenced = nullptr;

    void appendRef(Reference<Type>* const a_ref)const
    {
        Reference<Type>* pCurRef = const_cast<Reference<Type>*>(this);
        Reference<Type>* pLastRef = m_nextRef;
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
        m_referenced = nullptr;
        Reference<Type>* pLastRef = m_nextRef;
        while (pLastRef)
        {
            pLastRef->m_referenced = nullptr;
            pLastRef = pLastRef->m_nextRef;
        }
    }
       
    Reference(const Referenced<Type>& a_referenced)  : m_referenced{ a_referenced.Referenced<Type>::getRefPointer() }
    {
        if (m_referenced->m_referenceList == nullptr)
        {
            m_referenced->m_referenceList = this;
        }
        else
        {
            m_referenced->m_referenceList->appendRef(this);
        }
    }

    Reference(Type* const a_referenced) : m_referenced{ a_referenced->getRefPointer() }
    {
        if (m_referenced->m_referenceList == nullptr)
        {
            m_referenced->m_referenceList = this;
        }
        else
        {
            m_referenced->m_referenceList->appendRef(this);
        }
    }

public:
    Reference() = default;

    Reference(const Reference<Type>& a_other) : m_referenced{ a_other.m_referenced }
    {
        if (m_referenced->m_referenceList == nullptr)
        {
            m_referenced->m_referenceList = this;
        }
        else
        {
            a_other.appendRef(this);
        }
    }

    ~Reference()
    {
        if (m_referenced)
        {
            if (m_referenced->m_referenceList == this)
            {
                m_referenced->m_referenceList = m_nextRef;
            }
            else
            {
                m_prevRef->m_nextRef = m_nextRef;
            }
        }
    }

    Type* const operator -> ()
    {
        return static_cast<Type*>(m_referenced);
    }

    Type* const pointer() const
    {
        return static_cast<Type* const>(m_referenced);
    }

    Reference<Type>& operator = (const Reference<Type>& a_other)
    {
        if (m_referenced)
        {
            if (m_referenced->m_referenceList == this)
            {
                m_referenced->m_referenceList = m_nextRef;
            }
            else
            {
                m_prevRef->m_nextRef = m_nextRef;
            }
        }

        m_referenced = a_other.m_referenced;
        if (m_referenced->m_referenceList == nullptr)
        {
            m_referenced->m_referenceList = this;
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
        if (m_referenced)
        {
            Reference<Type>* pLastRef = m_referenced->m_referenceList;
            while (pLastRef)
            {
                counter++;
                pLastRef = pLastRef->m_nextRef;
            }
        }
        return counter;
    }

    bool valid()const
    {
        return m_referenced != nullptr;
    }

};

template<typename T>
Reference<T> make_ref(const T& a_object)
{
    return Reference<T>(a_object);
}