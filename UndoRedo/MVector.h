#pragma once
#include <algorithm>
#include <vector>
#include "MMemory.h"
#include "TIContainer.h"
#include "ContainerCell.h"
#include "RTTIDefinition_macros.h"
#include "TRecordContainer.h"
#include "UndoRedo.h"
#include "RecordSession.h"
#include "InputBinStream.h"

template<typename Type> requires std::is_base_of_v<IRecordObject, Type>
class MVector : public TIContainer<size_t>, private std::vector<ContainerCell<Type>>
{
    DECLARE_RTTI_DERIVED(1, MVector<Type>, IRecordObject)
private:
	using VectorBase = std::vector<ContainerCell<Type>>;
	ChangeAssertion<Type> m_itemCallback;
protected:
    void record_replace(const size_t& a_key, const IRecordObjectPtr& a_object)final
    {
        if (VectorBase::size() > a_key)
        {
            VectorBase::operator[](a_key) = MStatic_pointer_cast<Type>(a_object);
        }
        else
        {
            VectorBase::push_back(ContainerCell<Type>(MStatic_pointer_cast<Type>(a_object), m_itemCallback));
        }
    }

    void record_insert(const size_t& a_key, const IRecordObjectPtr& a_object)final
    {
        if (VectorBase::size() > a_key)
        {
            VectorBase::insert(begin() + a_key, ContainerCell<Type>(MStatic_pointer_cast<Type>(a_object), m_itemCallback));
        }
        else
        {
            VectorBase::push_back(ContainerCell<Type>(MStatic_pointer_cast<Type>(a_object), m_itemCallback));
        }
    }

    void record_eraseAt(const size_t& a_index) final
    {
        if (VectorBase::size() > a_index)
            VectorBase::erase(begin() + a_index);
    }

    void assert_ItemAdd(const MShared_ptr<Type>& a_pItem, const size_t a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            std::weak_ptr<TIContainer<size_t>> ptr = std::dynamic_pointer_cast<TIContainer<size_t>>(shared_from_this());
            curSession.addRecord(std::make_shared<TRecordInsert<size_t>>(ptr, a_index, a_pItem));
        }
    }

    void assert_ItemRemoved(const MShared_ptr<Type>& a_pItem, const size_t a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            std::weak_ptr<TIContainer<size_t>> ptr = std::dynamic_pointer_cast<TIContainer<size_t>>(shared_from_this());
            curSession.addRecord(std::make_shared<TRecordRemoved<size_t>>(ptr, a_index, a_pItem));
        }
    }

    void assert_ItemChanged(const ContainerCell<Type>* a_pItem, const MShared_ptr<Type>& a_pBefore, const MShared_ptr<Type>& a_pAfter)
    {
        if (m_bActiveCallback && UndoRedo::instance().sessionStarted())
        {
            const size_t index = a_pItem - VectorBase::data();
            RecordSession& curSession = UndoRedo::instance().currentSession();
            std::weak_ptr<TIContainer<size_t>> ptr = std::dynamic_pointer_cast<TIContainer<size_t>>(shared_from_this());
            curSession.addRecord(std::make_shared<TRecordChanged<size_t>>(ptr, index, a_pAfter, a_pBefore));
        }
    }

    bool m_bActiveCallback = true; /*!< active the assert_ItemChanged callback*/

public:
    MVector()
    {
        m_itemCallback = std::bind_front(&MVector<Type>::assert_ItemChanged, this);
    }
    explicit MVector(const size_t& size) : std::vector<ContainerCell<Type>>(size) {
        m_itemCallback = std::bind_front(&MVector<Type>::assert_ItemChanged, this);
    }

    size_t size() const noexcept { return VectorBase::size(); }
    constexpr void reserve(const size_t& a_cap) { VectorBase::reserve(a_cap); }
    bool empty()const noexcept { return VectorBase::empty(); }

    void clear()
    {
        size_t index = 0;
        for (const auto& obj : *this)
        {
            assert_ItemRemoved(obj, index);
            ++index;
        }
        VectorBase::clear();
    }

    template<typename ...Args>
    MVector& emplace_back(Args&& ...a_args)
    {
        VectorBase::emplace_back(m_itemCallback, a_args...);
        ContainerCell<Type>& pt = VectorBase::back();
        assert_ItemAdd(pt, size() - 1);
        return *this;
    }


    MVector& push_back(const std::shared_ptr<Type>& a_pointer)
    {
        VectorBase::emplace_back(a_pointer, m_itemCallback);
        assert_ItemAdd(a_pointer, size() - 1);
        return *this;
    }

    constexpr MVector& push_back(std::shared_ptr<Type>&& a_pointer)
    {
        VectorBase::push_back(ContainerCell<Type>(std::move(a_pointer), m_itemCallback));
        ContainerCell<Type>& pt = VectorBase::back();
        assert_ItemAdd(pt, size() - 1);
        return *this;
    }

    ContainerCell<Type>& operator[](const size_t& a_index)
    {
        return VectorBase::operator[](a_index);
    }

    using iterator = std::vector<ContainerCell<Type>>::iterator;
    iterator begin() { return VectorBase::begin(); }
    iterator end() { return VectorBase::end(); }


    using reverse_iterator = std::vector<ContainerCell<Type>>::reverse_iterator;
    reverse_iterator  rbegin() { return VectorBase::rbegin(); }
    reverse_iterator  rend() { return VectorBase::rend(); }

    using const_iterator = std::vector<ContainerCell<Type>>::const_iterator;
    const_iterator cbegin()const { return VectorBase::cbegin(); }
    const_iterator cend()const { return VectorBase::cend(); }
    iterator insert(const_iterator a_pos, const std::shared_ptr<Type>& a_object)
    {
        auto iter = VectorBase::insert(a_pos, ContainerCell<Type>(m_itemCallback, a_object));
        assert_ItemAdd((*iter), iter - begin());
        return iter;
    }

    constexpr iterator insert(const_iterator a_pos, std::shared_ptr<Type>&& a_object)
    {
        auto iter = VectorBase::insert(a_pos, ContainerCell<Type>(m_itemCallback, std::move(a_object)));
        assert_ItemAdd((*iter), std::distance(iter, begin()));
        return iter;
    }

    using const_reverse_iterator = std::vector<ContainerCell<Type>>::const_reverse_iterator;
    const_reverse_iterator crbegin()const { return VectorBase::crbegin(); }
    const_reverse_iterator crend()const { return VectorBase::crend(); }

    template<typename ...Args>
    MVector& emplace(const_iterator a_iter, Args&& ...args)
    {
        VectorBase::emplace(a_iter, m_itemCallback, std::make_shared<Type>(args...));
        assert_ItemAdd((*a_iter), std::distance(a_iter, begin()));
        return *this;
    }

    iterator erase(iterator a_pos)
    {
        assert_ItemRemoved((*a_pos), std::distance(a_pos, begin()));
        m_bActiveCallback = false;
        auto iter = VectorBase::erase(a_pos);
        m_bActiveCallback = true;
        return iter;
    }

    constexpr iterator erase(const_iterator a_pos)
    {
        m_bActiveCallback = false;
        assert_ItemRemoved((*a_pos), std::distance(a_pos, cbegin()));
        m_bActiveCallback = false;
        auto iter = VectorBase::erase(a_pos);
        m_bActiveCallback = true;
        return iter;
    }

    iterator erase(iterator  a_first, iterator a_last)
    {
        size_t index = 0;
        std::for_each(a_first, a_last - 1, [this, &index](auto&& a_obj)
            {
                assert_ItemRemoved(a_obj, index);
                ++index;
            });
        m_bActiveCallback = false;
        auto iter = VectorBase::erase(a_first, a_last);
        m_bActiveCallback = true;
        return iter;
    }

    constexpr iterator erase(const_iterator a_first, const_iterator a_last)
    {
        m_bActiveCallback = false;
        size_t index = 0;
        std::for_each(a_first, a_last - 1, [this](auto&& a_obj)
            {
                assert_ItemRemoved(a_obj, index);
                ++index;
            });
        m_bActiveCallback = false;
        auto iter = VectorBase::erase(a_first, a_last);
        m_bActiveCallback = true;
        return iter;
    }

    constexpr void pop_back()
    {
        ContainerCell<Type>& pt = VectorBase::back();
        assert_ItemRemoved(pt, VectorBase::size() - 1);
        VectorBase::pop_back();
    }

    virtual unsigned short load(IInputStream& a_stream)override
    {
        //Object::load(a_stream);
        size_t size;
        a_stream >> size;
        VectorBase::reserve(size);
        for (size_t i = 0; i < size; ++i)
        {
            // TODO
        }

        return definition()->version();
    }

    /*@brief save object to stream*/
    virtual bool save(IOutputStream& a_stream)const override
    {
        //Object::save(a_stream);
        a_stream << size();
        for (const auto& obj : (const VectorBase)*this)
            a_stream << obj.get()->uid();
        return true;
    }
};
