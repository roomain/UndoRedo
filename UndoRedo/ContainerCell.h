#pragma once
#include <functional>
#include "MMemory.h"

template<typename Type>
class ContainerCell;

template<typename Type>
using ChangeAssertion = std::function<void(const ContainerCell<Type>*, const MShared_ptr<Type>& , const MShared_ptr<Type>&)>;

template<typename Type>
class ContainerCell : public MShared_ptr<Type>
{
private:
	ChangeAssertion<Type> m_changeAssert;

public:
	ContainerCell() = default;

	explicit ContainerCell(ContainerCell<Type>&& a_other)noexcept : MShared_ptr<Type>(std::move(a_other)),
		m_changeAssert{ a_other.m_changeAssert }
	{
	}

	explicit ContainerCell(ContainerCell<Type>& a_other)noexcept : MShared_ptr<Type>(a_other),
		m_changeAssert{ a_other.m_changeAssert }
	{
	}

	explicit ContainerCell(const ChangeAssertion<Type>& a_assert) : m_changeAssert{ a_assert } {}

	explicit ContainerCell(const MShared_ptr<Type>& a_ptr, const ChangeAssertion<Type>& a_assert) :
		MShared_ptr<Type>(a_ptr), m_changeAssert{ a_assert } {}

	explicit ContainerCell(MShared_ptr<Type>&& a_ptr, const ChangeAssertion<Type>& a_assert)noexcept :
		MShared_ptr<Type>(std::move(a_ptr)), m_changeAssert{ a_assert } {}

	template<typename ...Args>
	ContainerCell(const ChangeAssertion<Type>& a_cb, Args&& ...a_args) :
		MShared_ptr<Type>(make_MShared<Type>(a_args...)), m_changeAssert{ a_cb } {}

	ContainerCell<Type>& operator = (const MShared_ptr<Type>& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (MShared_ptr<Type>&& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (const ContainerCell<Type>& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(*this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	ContainerCell<Type>& operator = (ContainerCell<Type>&& a_other)
	{
		if (this->get() != a_other.get())
		{
			if (m_changeAssert)
				m_changeAssert(this, *this, a_other);
			MShared_ptr<Type>::operator = (a_other);
		}
		return *this;
	}

	virtual ~ContainerCell()
	{
		//if constexpr (std::is_base_of_v<IRecordObject, Type>)
		//{
		//	int iCount = std::shared_ptr<Type>::use_count();
		//	if (std::shared_ptr<Type>::use_count() == 1)
		//	{
		//		auto p = std::shared_ptr<Type>::get();
		//		assertDeletion(const_cast<IRecordObject* const>(static_cast<const IRecordObject*>(std::shared_ptr<Type>::get())));
		//	}
		//}
	}
};